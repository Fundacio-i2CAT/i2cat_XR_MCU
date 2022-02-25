#pragma once

#include "ddk_iterable_action.h"

namespace ddk
{
namespace detail
{

class reversable_action_resolver
{
public:
	reversable_action_resolver(bool i_forward);

	template<typename ... Types>
	variant<Types...> resolve(const variant<Types...>& i_action) const;
	shift_action resolve(const shift_action& i_action) const;

protected:
	mutable bool m_forward;
};

class alternate_action_resolver : reversable_action_resolver
{
public:
	alternate_action_resolver(bool i_forward);
	alternate_action_resolver(size_t i_constrain, bool i_forward);

	template<typename ... Types>
	variant<Types...> resolve(const variant<Types...>& i_action) const;
	shift_action resolve(const shift_action& i_action) const;
	alternate_action_resolver operator()(size_t i_constrain) const;

private:
	const size_t m_constrain = 0;
	mutable int m_currIndex = 0;
};

}

extern const detail::reversable_action_resolver forward_order;
extern const detail::reversable_action_resolver reverse_order;
extern const detail::alternate_action_resolver alternate_order;

}

#include "ddk_iterable_action_resolver.inl"