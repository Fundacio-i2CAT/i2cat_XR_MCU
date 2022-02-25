#pragma once

#include "ddk_iaccess_critical_call_context.h"
#include "ddk_critical_section.h"

namespace ddk
{

template<typename Traits, IAccessProvider::Access>
struct AccessCriticalSection;

template<typename Traits>
class IAccessInterface : public IAccessProvider, protected IAccessCriticalCallContext<Traits>, protected Traits
{
	template<typename,IAccessProvider::Access>
	friend struct AccessCriticalSection;

public:
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef IAccessCriticalCallContext<Traits> access_call_context;

	template<IAccessProvider::Access ACCESS>
	typename AccessCriticalSection<Traits,ACCESS>::critical_section enterCriticalSection(Reentrancy i_reentrancy) const
	{
		return AccessCriticalSection<Traits,ACCESS>::enter(*this,i_reentrancy);
	}
	template<IAccessProvider::Access ACCESS>
	typename AccessCriticalSection<Traits,ACCESS>::critical_section askCriticalSection(Reentrancy i_reentrancy) const
	{
		return AccessCriticalSection<Traits,ACCESS>::tryToEnter(*this,i_reentrancy);
	}
	template<IAccessProvider::Access ACCESS>
	void leaveCriticalSection(typename AccessCriticalSection<Traits,ACCESS>::critical_section i_criticalSection) const
	{
		AccessCriticalSection<Traits, ACCESS>::leave(*this,std::move(i_criticalSection));
	}
	template<IAccessProvider::Access ACCESS>
	typename AccessCriticalSection<Traits,ACCESS>::critical_section enterCriticalSection(Reentrancy i_reentrancy)
	{
		return AccessCriticalSection<Traits, ACCESS>::enter(*this,i_reentrancy);
	}
	template<IAccessProvider::Access ACCESS>
	typename AccessCriticalSection<Traits,ACCESS>::critical_section askCriticalSection(Reentrancy i_reentrancy)
	{
		return AccessCriticalSection<Traits,ACCESS>::tryToEnter(*this,i_reentrancy);
	}
	template<IAccessProvider::Access ACCESS>
	void leaveCriticalSection(typename AccessCriticalSection<Traits,ACCESS>::critical_section i_criticalSection)
	{
		AccessCriticalSection<Traits, ACCESS>::leave(*this,std::move(i_criticalSection));
	}

private:
	virtual critical_context enterReader(Reentrancy) const = 0;
	virtual critical_context tryToEnterReader(Reentrancy) const = 0;
	virtual void leaveReader(critical_context) const = 0;
	virtual critical_context enterWriter(Reentrancy) = 0;
	virtual critical_context tryToEnterWriter(Reentrancy) = 0;
	virtual void leaveWriter(critical_context) = 0;
};

template<typename Traits>
struct AccessCriticalSection<Traits,IAccessProvider::READING>
{
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef IAccessCriticalCallContext<Traits> access_call_context;
	typedef typename std::add_const<access_call_context>::type const_call_context;
	typedef typename std::add_lvalue_reference<const_call_context>::type access_call_context_reference;

public:
	typedef const_critical_section<Traits> critical_section;

	static critical_section enter(const IAccessInterface<Traits>& i_accesInterface, Reentrancy i_reentrancy)
	{
		critical_context readerContext = i_accesInterface.enterReader(i_reentrancy);

		const critical_section_identifier _id(reinterpret_cast<size_t>(&i_accesInterface));

		return critical_section(static_cast<access_call_context_reference>(i_accesInterface),_id,std::move(readerContext));
	}
	static critical_section tryToEnter(const IAccessInterface<Traits>& i_accesInterface, Reentrancy i_reentrancy)
	{
		if(critical_context readerContext = i_accesInterface.tryToEnterReader(i_reentrancy))
		{
			const critical_section_identifier _id(reinterpret_cast<size_t>(&i_accesInterface));

			return critical_section(static_cast<access_call_context_reference>(i_accesInterface),_id,std::move(readerContext));
		}
		else
		{
			return empty_critical_section_impl;
		}
	}
	static void leave(const IAccessInterface<Traits>& i_accesInterface, critical_section i_criticalSection)
	{
		DDK_ASSERT_OR_LOG(i_criticalSection.get_id() == reinterpret_cast<size_t>(&i_accesInterface), "Leaving critical section from wrong access provider");

		i_accesInterface.leaveReader(i_criticalSection.extract_context());
	}
};

template<typename Traits>
struct AccessCriticalSection<Traits,IAccessProvider::WRITING>
{
	typedef critical_section<Traits> critical_section;
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef IAccessCriticalCallContext<Traits> access_call_context;
	typedef typename std::add_lvalue_reference<access_call_context>::type access_call_context_reference;

	static critical_section enter(IAccessInterface<Traits>& i_accesInterface, Reentrancy i_reentrancy)
	{
		critical_context writerContext = i_accesInterface.enterWriter(i_reentrancy);

		const critical_section_identifier _id(reinterpret_cast<size_t>(&i_accesInterface));

		return critical_section(static_cast<access_call_context_reference>(i_accesInterface),_id,std::move(writerContext));
	}
	static critical_section tryToEnter(IAccessInterface<Traits>& i_accesInterface, Reentrancy i_reentrancy)
	{
		if(critical_context writerContext = i_accesInterface.tryToEnterWriter(i_reentrancy))
		{
			const critical_section_identifier _id(reinterpret_cast<size_t>(&i_accesInterface));

			return critical_section(static_cast<access_call_context_reference>(i_accesInterface),_id,std::move(writerContext));
		}
		else
		{
			return empty_critical_section_impl;
		}
	}
	static void leave(IAccessInterface<Traits>& i_accesInterface, critical_section i_criticalSection)
	{
		DDK_ASSERT_OR_LOG(i_criticalSection.get_id() == reinterpret_cast<size_t>(&i_accesInterface), "Leaving critical section from wrong access provider");

		i_accesInterface.leaveWriter(i_criticalSection.extract_context());
	}
};

}