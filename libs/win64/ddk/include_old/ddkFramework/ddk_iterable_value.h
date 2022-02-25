#pragma once

#include "ddk_iterable_traits.h"
#include "ddk_iterable_interface.h"
#include <type_traits>

namespace ddk
{
namespace detail
{

struct iterable_value_base{};

}

template<typename Traits>
struct iterable_value : public detail::iterable_value_base
{
    template<typename...>
    friend struct random_accessed_value;
    template<typename...>
    friend struct const_random_accessed_value;
    template<typename...>
    friend struct bidirectional_value;
    template<typename...>
    friend struct const_bidirectional_value;
    template<typename...>
    friend struct forwarded_value;
    template<typename...>
    friend struct const_forwarded_value;

	friend inline size_t value_position(const iterable_value& i_value)
	{
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return currState.position();
	}

public:
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::pointer pointer;
    typedef typename Traits::const_pointer const_pointer;
    typedef typename Traits::action action;

    iterable_value(reference i_value, const function<reference(action)>& i_resolver, detail::iterable_interface& i_iterableInterface);
    template<typename Reference, typename Return, typename Action>
    iterable_value(Reference&& i_value, const function<Return(Action)>& i_resolver, detail::iterable_interface& i_iterableInterface);
    iterable_value(const iterable_value&) = delete;
    iterable_value(iterable_value&&) = default;
    iterable_value& operator=(const iterable_value&) = delete;
    iterable_value& operator=(iterable_value&&) = default;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
    bool operator==(const_reference i_value) const;
    bool operator!=(const_reference i_value) const;

protected:
    pointer m_value;
    function<reference(action)> m_resolver;
    detail::iterable_interface& m_iterableInterface;
};

template<typename...>
struct random_accessed_value;

template<typename T>
struct random_accessed_value<T> : public iterable_value<detail::random_access_iterable_traits<T>>
{
    friend inline std::pair<random_accessed_value,erase_result> erase_value(random_accessed_value i_value)
    {
        action_state_lent_ptr currActionState = i_value.m_iterableInterface.get_action_state();

        return std::make_pair(random_accessed_value(eval(i_value.m_resolver,erase_place),i_value.m_resolver), currActionState->template forward_result<erase_result>());
    }
    template<typename ... Args>
    friend inline std::pair<random_accessed_value,add_result> add_value(random_accessed_value i_value, Args&& ... i_args)
    {
        action_state_lent_ptr currActionState = i_value.m_iterableInterface.get_action_state();

        return std::make_pair(random_accessed_value(eval(i_value.m_resolver,add_place(std::forward<Args>(i_args) ...)),i_value.m_resolver), currActionState->template forward_result<add_result>());
    }
    friend inline random_accessed_value next_value(random_accessed_value i_value)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,go_forward_action(currState.position())),i_value.m_resolver };
    }
    friend inline random_accessed_value prev_value(random_accessed_value i_value)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,go_backward_action(currState.position())),i_value.m_resolver };
    }
    friend inline random_accessed_value shift_value(random_accessed_value i_value, int i_shift)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,shift_action(currState.position(),i_shift)),i_value.m_resolver };
    }

public:
    using iterable_value<detail::random_access_iterable_traits<T>>::iterable_value;
    random_accessed_value(const random_accessed_value&) = delete;
    random_accessed_value(random_accessed_value&&) = default;
    template<typename TT>
    random_accessed_value(random_accessed_value<TT>&&);

    random_accessed_value& operator=(random_accessed_value&&) = default;
};
template<typename Type, typename ... Types>
struct random_accessed_value<Type,Types...> : public random_accessed_value<values_tuple<Type,Types...>>
{
    using random_accessed_value<values_tuple<Type,Types...>>::random_accessed_value;
};

template<typename...>
struct const_random_accessed_value;

template<typename T>
struct const_random_accessed_value<T> : public iterable_value<detail::random_access_iterable_traits<T>>
{
    template<typename...>
    friend struct const_random_accessed_value;
    friend inline const_random_accessed_value next_value(const_random_accessed_value i_value)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,go_forward_action(currState.position())),i_value.m_resolver };
    }
    friend inline const_random_accessed_value prev_value(const_random_accessed_value i_value)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,go_backward_action(currState.position())),i_value.m_resolver };
    }
    friend inline const_random_accessed_value shift_value(const_random_accessed_value i_value, int i_shift)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,shift_action(currState.position(),i_shift)),i_value.m_resolver };
    }

public:
    using iterable_value<detail::random_access_iterable_traits<T>>::iterable_value;
    const_random_accessed_value(const const_random_accessed_value&) = delete;
    const_random_accessed_value(const_random_accessed_value&&) = default;
    template<typename TT>
    const_random_accessed_value(const_random_accessed_value<TT>&& other);
    template<typename TT>
    const_random_accessed_value(random_accessed_value<TT>&& other);

    const_random_accessed_value& operator=(const_random_accessed_value&&) = default;
};
template<typename Type, typename ... Types>
struct const_random_accessed_value<Type,Types...> : public const_random_accessed_value<values_tuple<Type,Types...>>
{
    using const_random_accessed_value<values_tuple<Type,Types...>>::const_random_accessed_value;
};

template<typename...>
struct bidirectional_value;

template<typename T>
struct bidirectional_value<T> : public iterable_value<detail::bidirectional_iterable_traits<T>>
{
    friend inline std::pair<bidirectional_value,erase_result> erase_value(bidirectional_value i_value)
    {
        action_state_lent_ptr currActionState = i_value.m_iterableInterface.get_action_state();

        return std::make_pair(bidirectional_value(eval(i_value.m_resolver,erase_place),i_value.m_resolver ), currActionState->template forward_result<erase_result>());
    }
    template<typename ... Args>
    friend inline std::pair<bidirectional_value,add_result> add_value(bidirectional_value i_value, Args&& ... i_args)
    {
        action_state_lent_ptr currActionState = i_value.m_iterableInterface.get_action_state();

        return std::make_pair(bidirectional_value(eval(i_value.m_resolver,add_place(std::forward<Args>(i_args) ...)),i_value.m_resolver), currActionState->template forward_result<add_result>());
    }
    friend inline bidirectional_value next_value(bidirectional_value i_value)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,go_forward_action(currState.position())),i_value.m_resolver };
    }
    friend inline bidirectional_value prev_value(bidirectional_value i_value)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,go_backward_action(currState.position())),i_value.m_resolver };
    }
public:
    using iterable_value<detail::bidirectional_iterable_traits<T>>::iterable_value;
    bidirectional_value(const bidirectional_value&) = delete;
    bidirectional_value(bidirectional_value&&) = default;
    template<typename TT>
    bidirectional_value(bidirectional_value<TT>&&);
    template<typename TT>
    bidirectional_value(random_accessed_value<TT>&&);
    template<typename TT>
    bidirectional_value(const_random_accessed_value<TT>&&);

    bidirectional_value& operator=(bidirectional_value&&) = default;
};
template<typename Type, typename ... Types>
struct bidirectional_value<Type,Types...> : public bidirectional_value<values_tuple<Type,Types...>>
{
    using bidirectional_value<values_tuple<Type,Types...>>::bidirectional_value;
};

template<typename...>
struct const_bidirectional_value;

template<typename T>
struct const_bidirectional_value<T> : public iterable_value<detail::bidirectional_iterable_traits<T>>
{
    template<typename...>
    friend struct const_bidirectional_value;
    friend inline const_bidirectional_value next_value(const_bidirectional_value i_value)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,go_forward_action(currState.position())),i_value.m_resolver };
    }
    friend inline const_bidirectional_value prev_value(const_bidirectional_value i_value)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,go_backward_action(currState.position())),i_value.m_resolver };
    }

public:
    using iterable_value<detail::bidirectional_iterable_traits<T>>::iterable_value;
    const_bidirectional_value(const const_bidirectional_value&) = delete;
    const_bidirectional_value(const_bidirectional_value&&) = default;
    template<typename TT>
    const_bidirectional_value(const_bidirectional_value<TT>&& other);
    template<typename TT>
    const_bidirectional_value(const_random_accessed_value<TT>&&);

    const_bidirectional_value& operator=(const_bidirectional_value&&) = default;
};
template<typename Type, typename ... Types>
struct const_bidirectional_value<Type,Types...> : public const_bidirectional_value<values_tuple<Type,Types...>>
{
    using const_bidirectional_value<values_tuple<Type,Types...>>::const_bidirectional_value;
    const_bidirectional_value(const values_tuple<Type,Types...> i_value);
};

template<typename...>
struct forwarded_value;

template<typename T>
struct forwarded_value<T> : public iterable_value<detail::forward_iterable_traits<T>>
{
    friend inline std::pair<forwarded_value,erase_result> erase_value(forwarded_value i_value)
    {
        action_state_lent_ptr currActionState = i_value.m_iterableInterface.get_action_state();

        return std::make_pair(forwarded_value(eval(i_value.m_resolver,erase_place),i_value.m_resolver),currActionState->template forward_result<erase_result>());
    }
    template<typename ... Args>
    friend inline std::pair<forwarded_value,add_result> add_value(forwarded_value i_value, Args&& ... i_args)
    {
        action_state_lent_ptr currActionState = i_value.m_iterableInterface.get_action_state();

        return std::make_pair(forwarded_value(eval(i_value.m_resolver,add_place(std::forward<Args>(i_args) ...)),i_value.m_resolver), currActionState->template forward_result<add_result>());
    }
    friend inline forwarded_value next_value(forwarded_value i_value)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,go_forward_action(currState.position())),i_value.m_resolver };
    }
public:
    using iterable_value<detail::forward_iterable_traits<T>>::iterable_value;
    forwarded_value(const forwarded_value&) = delete;
    forwarded_value(forwarded_value&&) = default;
    template<typename TT>
    forwarded_value(forwarded_value<TT>&& other);
    template<typename TT>
    forwarded_value(bidirectional_value<TT>&&);
    template<typename TT>
    forwarded_value(const_bidirectional_value<TT>&&);
    template<typename TT>
    forwarded_value(random_accessed_value<TT>&&);
    template<typename TT>
    forwarded_value(const_random_accessed_value<TT>&&);

    forwarded_value& operator=(forwarded_value&&) = default;
};
template<typename Type, typename ... Types>
struct forwarded_value<Type,Types...> : public forwarded_value<values_tuple<Type,Types...>>
{
    using forwarded_value<values_tuple<Type,Types...>>::forwarded_value;
};

template<typename...>
struct const_forwarded_value;

template<typename T>
struct const_forwarded_value<T> : public iterable_value<detail::forward_iterable_traits<T>>
{
    template<typename...>
    friend struct const_forwarded_value;
    friend inline const_forwarded_value next_value(const_forwarded_value i_value)
    {
		const iterable_state& currState = i_value.m_iterableInterface.get_state();

		return { eval(i_value.m_resolver,go_forward_action(currState.position())),i_value.m_resolver };
    }

public:
    using iterable_value<detail::forward_iterable_traits<T>>::iterable_value;
    const_forwarded_value(const const_forwarded_value&) = delete;
    const_forwarded_value(const_forwarded_value&&) = default;
    template<typename TT>
    const_forwarded_value(const_forwarded_value<TT>&& other);
    template<typename TT>
    const_forwarded_value(const_bidirectional_value<TT>&&);
    template<typename TT>
    const_forwarded_value(const_random_accessed_value<TT>&&);

    const_forwarded_value& operator=(const_forwarded_value&&) = default;
};
template<typename Type, typename ... Types>
struct const_forwarded_value<Type,Types...> : public const_forwarded_value<values_tuple<Type,Types...>>
{
    using const_forwarded_value<values_tuple<Type,Types...>>::const_forwarded_value;
};

template<typename Value, typename ... Args>
inline Value make_iterable_value(Args&& ... i_args);

}

#include "ddk_iterable_value.inl"
