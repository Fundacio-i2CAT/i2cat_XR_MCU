#pragma once

#include <iterator>
#include <tuple>
#include "ddk_template_helper.h"

//bidirectional multi iterators

namespace ddk
{

template<typename ... Iterators>
class multi_iterator
{
    static const size_t s_numTypes = mpl::get_num_types<Iterators...>();
    typedef typename mpl::create_range_rank<0,s_numTypes>::type type_range_rank;

public:
	typedef std::tuple<typename Iterators::reference ...> reference;
	typedef typename std::add_const<reference>::type const_reference;
	typedef std::tuple<typename Iterators::pointer ...> pointer;
	typedef typename std::add_const<pointer>::type const_pointer;
	typedef size_t distance_type;
	typedef std::ptrdiff_t difference_type;
	typedef std::tuple<typename Iterators::value_type ...> value_type;
	typedef std::bidirectional_iterator_tag iterator_category;

	multi_iterator(const Iterators& ... i_iterators)
	: m_iterators(i_iterators ... )
	{
	}
	reference operator*()
	{
        return get_ref(type_range_rank{});
	}
	const_reference operator*() const
	{
        return get_ref(type_range_rank{});
	}
	pointer operator->()
	{
        return get_ptr(type_range_rank{});
	}
	const_pointer operator->() const
	{
        return get_ptr(type_range_rank{});
	}
	bool operator==(const multi_iterator& other) const
	{
        return is_eq(type_range_rank{},other);
	}
	bool operator!=(const multi_iterator& other) const
	{
        return is_neq(type_range_rank{},other);
	}
	//forward iterator
	multi_iterator& operator++()
	{
        incr(type_range_rank{});

		return *this;
	}
	multi_iterator operator++(int)
	{
		multi_iterator res = *this;

        incr(type_range_rank{});

		return res;
	}
	//backward iterator
	multi_iterator& operator--()
	{
        decr(type_range_rank{});

		return *this;
	}
	multi_iterator operator--(int)
	{
		multi_iterator res = res;

        decr(type_range_rank{});

		return res;
	}

private:
    template<size_t ... Seq>
	inline reference get_ref(const mpl::sequence<Seq...>&)
	{
		return std::make_tuple(*std::get<Seq>(m_iterators)...);
	}
    template<size_t ... Seq>
	inline const_reference get_ref(const mpl::sequence<Seq...>&) const
	{
		return std::make_tuple(*std::get<Seq>(m_iterators)...);
	}
    template<size_t ... Seq>
	inline pointer get_ptr(const mpl::sequence<Seq...>&)
	{
		return std::make_tuple(&(*std::get<Seq>(m_iterators))...);
	}
    template<size_t ... Seq>
	inline const_pointer get_ptr(const mpl::sequence<Seq...>&) const
	{
		return std::make_tuple(&(*std::get<Seq>(m_iterators))...);
	}
    template<size_t ... Seq>
	inline bool is_eq(const mpl::sequence<Seq...>& i_seq, const multi_iterator& other) const
	{
		bool res[i_seq.size] = { std::get<Seq>(m_iterators) == std::get<Seq>(other.m_iterators) ... };

        for(size_t tupleIndex=0;tupleIndex<i_seq.size;++tupleIndex)
        {
            if(res[tupleIndex] == false)
            {
                return false;
            }
        }

        return true;
	}
    template<size_t ... Seq>
	inline bool is_neq(const mpl::sequence<Seq...>& i_seq, const multi_iterator& other) const
	{
		bool res[i_seq.size] = { std::get<Seq>(m_iterators) != std::get<Seq>(other.m_iterators) ... };

        for(size_t tupleIndex=0;tupleIndex<i_seq.size;++tupleIndex)
        {
            if(res[tupleIndex] == true)
            {
                return true;
            }
        }

        return false;
	}
    template<size_t ... Seq>
	inline void incr(const mpl::sequence<Seq...>& i_seq)
	{
		void* res[i_seq.size] = { (void*)&(++std::get<Seq>(m_iterators)) ... };

        UNUSED(res)
	}
    template<size_t ... Seq>
	inline void decr(const mpl::sequence<Seq...>& i_seq)
	{
		void* res[i_seq.size] = { (void*)&(--std::get<Seq>(m_iterators)) ... };

        UNUSED(res)
	}

    std::tuple<Iterators...> m_iterators;
};

}
