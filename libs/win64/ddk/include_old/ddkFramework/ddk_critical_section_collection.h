#pragma once

#include "ddk_critical_section.h"
#include "ddk_reference_counter.h"
#include "ddk_template_helper.h"

#include <tuple>

namespace ddk
{
namespace detail
{

template<IAccessProvider::Access,typename>
struct resolve_critical_access_type;

template<typename T>
struct resolve_critical_access_type<IAccessProvider::READING,T>
{
	typedef typename T::const_critical_section type;
};
template<typename T>
struct resolve_critical_access_type<IAccessProvider::WRITING,T>
{
	typedef typename T::critical_section type;
};

}

//dynamic version
template<IAccessProvider::Access ACCESS, typename T>
class CriticalSectionCollectorDyn
{
	template<typename>
	friend class AccessProviderCollectorDyn;

	typedef typename detail::resolve_critical_access_type<ACCESS,T>::type critical_section_t;
	typedef std::vector<critical_section_t> vector_t;

public:
	CriticalSectionCollectorDyn()
	: m_criticalSections()
	{};
	template<IAccessProvider::Access AACCESS>
	CriticalSectionCollectorDyn(CriticalSectionCollectorDyn<AACCESS,T>&& other)
	: m_criticalSections(std::move(other.m_criticalSections))
	{
	}
	CriticalSectionCollectorDyn& operator=(CriticalSectionCollectorDyn&& other)
	{
		m_criticalSections = std::move(other);

		return *this;
	}
	typename vector_t::iterator begin()
	{
		return m_criticalSections.begin();
	}
	typename vector_t::const_iterator begin() const
	{
		return m_criticalSections.begin();
	}
	typename vector_t::iterator end()
	{
		return m_criticalSections.end();
	}
	typename vector_t::const_iterator end() const
	{
		return m_criticalSections.end();
	}
	typename detail::resolve_critical_access_type<ACCESS,T>::type& operator[](size_t i_index)
	{
		DDK_ASSERT(i_index < m_criticalSections.size(), "Index out of bounds");
		return m_criticalSections[i_index];
	}
	const typename detail::resolve_critical_access_type<ACCESS,T>::type& operator[](size_t i_index) const
	{
		DDK_ASSERT(i_index < m_criticalSections.size(), "Index out of bounds");
		return m_criticalSections[i_index];
	}
	size_t size() const
	{
		return m_criticalSections.size();
	}
	bool empty() const
	{
		return m_criticalSections.empty();
	}

private:
	void push_back(critical_section_t&& i_criticalSection)
	{
		m_criticalSections.push_back(std::move(i_criticalSection));
	}
	typename std::add_rvalue_reference<critical_section_t>::type extract_by_index(size_t i_index)
	{
		return std::move(m_criticalSections[i_index]);
	}
	void clear()
	{
		m_criticalSections.clear();
	}

	vector_t m_criticalSections;
};
template<typename T>
class AccessProviderCollectorDyn
{
public:
	typedef AccessProviderCollectorDyn<T> collector_t;
	typedef CriticalSectionCollectorDyn<IAccessProvider::WRITING,T> critical_section;
	typedef CriticalSectionCollectorDyn<IAccessProvider::READING,T> const_critical_section;

	template<typename Iterator>
	AccessProviderCollectorDyn(Iterator i_itInit,Iterator i_itEnd)
	{
		for (; i_itInit != i_itEnd; ++i_itInit)
		{
			m_accesProviders.push_back(&(*i_itInit));
		}
	}
	template<IAccessProvider::Access ACCESS>
	typename detail::resolve_critical_access_type<ACCESS,collector_t>::type enterCriticalSection(Reentrancy i_reentrancy) const
	{
		typedef typename detail::resolve_critical_access_type<ACCESS,collector_t>::type critical_section_t;
		critical_section_t acquiredCriticalSections;
		const size_t numProviders = m_accesProviders.size();
		bool done = numProviders > 0;

		while (!done);
		{
			T* firstProvider = m_accesProviders[0];
			acquiredCriticalSections.push_back(firstProvider->enterCriticalSection<ACCESS>(i_reentrancy));

			for (size_t providerIndex = 1; providerIndex < numProviders; ++providerIndex)
			{
				if (typename detail::resolve_critical_access_type<ACCESS,T>::type nthCriticalSection = m_accesProviders[providerIndex]->askCriticalSection<ACCESS>(i_reentrancy))
				{
					acquiredCriticalSections.push_back(std::move(nthCriticalSection));
					done = true;
				}
				else
				{
					for (int acquireProviderIndex=providerIndex-1;acquireProviderIndex>=0;--acquireProviderIndex)
					{
						m_accesProviders[acquireProviderIndex]->leaveCriticalSection<ACCESS>(acquiredCriticalSections.extract_by_index(acquireProviderIndex));
					}
					firstProvider->leaveCriticalSection<ACCESS>(acquiredCriticalSections.extract_by_index(0));
					acquiredCriticalSections.clear();
					done = false;
					break;
				}
			}
		}

		return acquiredCriticalSections;
	}
	template<IAccessProvider::Access ACCESS>
	typename detail::resolve_critical_access_type<ACCESS,collector_t>::type askCriticalSection(Reentrancy i_reentrancy) const
	{
		typedef typename detail::resolve_critical_access_type<ACCESS,collector_t>::type critical_section_t;
		critical_section_t acquiredCriticalSections;

		const size_t numProviders = m_accesProviders.size();
		for (size_t providerIndex = 0; providerIndex < numProviders; ++providerIndex)
		{
			if (typename detail::resolve_critical_access_type<ACCESS,T>::type nthCriticalSection = m_accesProviders[providerIndex]->askCriticalSection<ACCESS>(i_reentrancy))
			{
				acquiredCriticalSections.push_back(std::move(nthCriticalSection));
			}
			else if (providerIndex > 0)
			{
				for (int acquiredCriticalSectionIndex = providerIndex - 1; acquiredCriticalSectionIndex >= 0; --acquiredCriticalSectionIndex)
				{
					m_accesProviders[acquiredCriticalSectionIndex]->leaveCriticalSection<ACCESS>(acquiredCriticalSections.extract_by_index(acquiredCriticalSectionIndex));
				}
				acquiredCriticalSections.clear();
			}
		}

		return acquiredCriticalSections;
	}
	template<IAccessProvider::Access ACCESS>
	void leaveCriticalSection(typename detail::resolve_critical_access_type<ACCESS,AccessProviderCollectorDyn<T>>::type i_criticalSection) const
	{
		DDK_ASSERT(i_criticalSection.size() == m_accesProviders.size(), "Unconsistent num of critical sections");

		const size_t numCriticalSections = i_criticalSection.size();
		for (int acquiredProviderIndex=numCriticalSections-1;acquiredProviderIndex>=0;--acquiredProviderIndex)
		{
			m_accesProviders[acquiredProviderIndex]->leaveCriticalSection<ACCESS>(i_criticalSection.extract_by_index(acquiredProviderIndex));
		}
		i_criticalSection.clear();
	}

private:
	std::vector<T*> m_accesProviders;
};

// variadic providers version
template<IAccessProvider::Access ACCESS, typename ... Types>
class CriticalSectionCollector
{
	template<typename...>
	friend class AccessProviderCollector;

	static const size_t s_numTypes = mpl::get_num_types<Types...>();
	typedef std::tuple<typename detail::resolve_critical_access_type<ACCESS,Types>::type ...> tuple_t;

public:
	CriticalSectionCollector()
	: m_criticalSections(ddk::mpl::construct_type<typename detail::resolve_critical_access_type<ACCESS,Types>::type>::with(empty_critical_section) ...)
	{
	}
	template<typename ... Args>
	CriticalSectionCollector(Args&& ... i_args)
	: m_criticalSections(std::forward<Args>(i_args)...)
	{
	}
	CriticalSectionCollector(const CriticalSectionCollector&) = delete;
	CriticalSectionCollector(CriticalSectionCollector<ACCESS,Types...>&& other)
	: m_criticalSections(std::move(other.m_criticalSections))
	{
	}
	template<IAccessProvider::Access AACCESS>
	CriticalSectionCollector(CriticalSectionCollector<AACCESS,Types...>&& other)
	: m_criticalSections(std::move(other.m_criticalSections))
	{
	}
	CriticalSectionCollector& operator=(CriticalSectionCollector&& other)
	{
		m_criticalSections = std::move(other.m_criticalSections);

		return *this;
	}
	template<typename Provider>
	typename detail::resolve_critical_access_type<ACCESS,Provider>::type& get()
	{
		return std::get<mpl::nth_pos_of_type<Provider,Types...>>(m_criticalSections);
	}
	template<typename Provider>
	const typename detail::resolve_critical_access_type<ACCESS,Provider>::type& get() const
	{
		return std::get<mpl::nth_pos_of_type<Provider,Types...>>(m_criticalSections);
	}
	template<size_t Index>
	typename std::add_lvalue_reference<typename detail::resolve_critical_access_type<ACCESS,typename mpl::nth_type_of<Index,Types...>::type>::type>::type get()
	{
		return std::get<Index>(m_criticalSections);
	}
	template<size_t Index>
	typename std::add_lvalue_reference<typename std::add_const<typename detail::resolve_critical_access_type<ACCESS,typename mpl::nth_type_of<Index,Types...>::type>::type>::type>::type get() const
	{
		return std::get<Index>(m_criticalSections);
	}
	bool empty() const
	{
		return _empty(typename mpl::create_range_rank<0,s_numTypes>::type{});
	}

private:
	template<int ... Seq>
	bool _empty() const
	{
		const bool s_empty[s_numTypes] = { std::get<Seq>(m_criticalSections) ... };

		for(size_t csIndex=0;csIndex<s_numTypes;++csIndex)
		{
			if(s_empty[csIndex] == false)
			{
				return false;
			}
		}

		return true;
	}
	template<size_t Index>
	typename std::add_rvalue_reference<typename mpl::nth_type_of<Index,typename detail::resolve_critical_access_type<ACCESS,Types>::type ...>::type>::type extract_by_index()
	{
		return std::move(std::get<Index>(m_criticalSections));
	}
	tuple_t m_criticalSections;
};

template<typename ... Types>
class AccessProviderCollector
{
	static const size_t s_numTypes = mpl::get_num_types<Types...>();

public:
	typedef AccessProviderCollector<Types...> collector_t;
	typedef CriticalSectionCollector<IAccessProvider::WRITING,Types...> critical_section;
	typedef CriticalSectionCollector<IAccessProvider::READING,Types...> const_critical_section;

	template<typename ... Args>
	AccessProviderCollector(Args&& ... i_args)
	: m_accesProviders(std::forward<Args>(i_args) ...)
	{
	}
	template<IAccessProvider::Access ACCESS>
	typename detail::resolve_critical_access_type<ACCESS,collector_t>::type enterCriticalSection(Reentrancy i_reentrancy) const
	{
		typedef typename mpl::create_range_rank<0,s_numTypes>::type range_type;

		return _enterCriticalSection<ACCESS>(i_reentrancy,range_type{});
	}
	template<IAccessProvider::Access ACCESS>
	typename detail::resolve_critical_access_type<ACCESS,collector_t>::type askCriticalSection(Reentrancy i_reentrancy) const
	{
		typedef typename mpl::create_range_rank<0,s_numTypes>::type range_type;

		return _askCriticalSection<ACCESS>(i_reentrancy,range_type{});
	}
	template<IAccessProvider::Access ACCESS>
	void leaveCriticalSection(typename detail::resolve_critical_access_type<ACCESS,AccessProviderCollector<Types...>>::type i_criticalSection) const
	{
		typedef typename mpl::create_range_rank<0,s_numTypes>::type range_type;

		_leaveCriticalSection<ACCESS>(i_criticalSection,range_type{});
	}

private:
	template<IAccessProvider::Access ACCESS, size_t ... Seq>
	typename detail::resolve_critical_access_type<ACCESS,collector_t>::type _enterCriticalSection(Reentrancy i_reentrancy, const mpl::sequence<Seq...>& i_seq) const
	{
		typedef typename detail::resolve_critical_access_type<ACCESS,collector_t>::type critical_section_t;
		typedef void(AccessProviderCollector<Types...>::*enter_critical_func)(Reentrancy,critical_section_t&) const;
		typedef bool(AccessProviderCollector<Types...>::*ask_critical_func)(Reentrancy,critical_section_t&) const;
		static const enter_critical_func s_enterFunc[s_numTypes] = { &AccessProviderCollector<Types...>::__enterCriticalSection<Seq,ACCESS> ...};
		static const ask_critical_func s_askFunc[s_numTypes] = { &AccessProviderCollector<Types...>::__askCriticalSection<Seq,ACCESS> ...};
		critical_section_t res;
		bool alreadyLocked = false;
		size_t enterIndex = 0;
		size_t lastEnterIndex = 0;

		do
		{
			alreadyLocked = false;

			(this->*s_enterFunc[enterIndex])(i_reentrancy,res);

			lastEnterIndex = enterIndex;

			for(size_t seqIndex=1;seqIndex<s_numTypes;++seqIndex)
			{
				const size_t currIndex = (enterIndex + seqIndex)%s_numTypes;

				if((this->*s_askFunc[currIndex])(i_reentrancy,res) == false)
				{
					_leaveCriticalSection<ACCESS>(res,i_seq,enterIndex,lastEnterIndex);

					alreadyLocked = true;

					enterIndex = currIndex;

					break;
				}
				else
				{
					lastEnterIndex = currIndex;
				}
			}
		}
		while(alreadyLocked);

		return std::move(res);
	}
	template<IAccessProvider::Access ACCESS, size_t ... Seq>
	typename detail::resolve_critical_access_type<ACCESS,collector_t>::type _askCriticalSection(Reentrancy i_reentrancy, const mpl::sequence<Seq...>& i_seq) const
	{
		typedef typename detail::resolve_critical_access_type<ACCESS,collector_t>::type critical_section_t;
		typedef bool(AccessProviderCollector<Types...>::*ask_func)(Reentrancy,critical_section_t&) const;
		static const ask_func s_askFunc[s_numTypes] = { &AccessProviderCollector<Types...>::__askCriticalSection<Seq,ACCESS> ...};
		critical_section_t res;
		bool alreadyLocked = false;

		for(size_t seqIndex=0;seqIndex<s_numTypes;++seqIndex)
		{
			if((this->*s_askFunc[seqIndex])(i_reentrancy,res) == false)
			{
				if(seqIndex > 0)
				{
					_leaveCriticalSection<ACCESS>(res,i_seq,0,seqIndex-1);
				}

				break;
			}
		}

		return std::move(res);
	}
	template<IAccessProvider::Access ACCESS, size_t ... Seq>
	void _leaveCriticalSection(typename detail::resolve_critical_access_type<ACCESS,collector_t>::type& i_criticalSection, const mpl::sequence<Seq...>&, size_t i_criticalSectionIndexMin = 0, size_t i_criticalSectionIndexMax = s_numTypes - 1) const
	{
		typedef typename detail::resolve_critical_access_type<ACCESS,collector_t>::type critical_section_t;
		typedef void(AccessProviderCollector<Types...>::*leave_func)(critical_section_t&) const;
		static const leave_func s_leaveFunc[s_numTypes] = { &AccessProviderCollector<Types...>::__leaveCriticalSection<Seq,ACCESS> ...};

		int indexDistance = (static_cast<int>(i_criticalSectionIndexMax) - static_cast<int>(i_criticalSectionIndexMin)) % s_numTypes;
		while(indexDistance >= 0)
		{
			size_t seqIndex = (i_criticalSectionIndexMin + indexDistance) % s_numTypes;

			(this->*s_leaveFunc[seqIndex])(i_criticalSection);

			--indexDistance;
		}
	}
	template<size_t Seq, IAccessProvider::Access ACCESS>
	void __enterCriticalSection(Reentrancy i_reentrancy, typename detail::resolve_critical_access_type<ACCESS,collector_t>::type& i_criticalSection) const
	{
		i_criticalSection.template get<Seq>() = std::get<Seq>(m_accesProviders).enterCriticalSection<ACCESS>(i_reentrancy);
	}
	template<size_t Seq, IAccessProvider::Access ACCESS>
	bool __askCriticalSection(Reentrancy i_reentrancy, typename detail::resolve_critical_access_type<ACCESS,collector_t>::type& i_criticalSection) const
	{
		if(auto cs = std::get<Seq>(m_accesProviders).askCriticalSection<ACCESS>(i_reentrancy))
		{
			i_criticalSection.template get<Seq>() = std::move(cs);

			return true;
		}

		return false;
	}
	template<size_t Seq, IAccessProvider::Access ACCESS>
	void __leaveCriticalSection(typename detail::resolve_critical_access_type<ACCESS,collector_t>::type& i_criticalSection) const
	{
		std::get<Seq>(m_accesProviders).template leaveCriticalSection<ACCESS>(i_criticalSection.template extract_by_index<Seq>());
	}

	std::tuple<typename std::add_lvalue_reference<Types>::type ...> m_accesProviders;
};

namespace mpl
{

template<IAccessProvider::Access ACCESS, typename T>
struct is_copy_constructible<CriticalSectionCollectorDyn<ACCESS,T>>
{
	static const bool value = false;
};
template<IAccessProvider::Access ACCESS, typename ... Types>
struct is_copy_constructible<CriticalSectionCollector<ACCESS,Types...>>
{
	static const bool value = false;
};

}
}
