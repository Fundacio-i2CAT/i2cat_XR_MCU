#pragma once

#include "ddk_variant.h"

namespace ddk
{

struct iterable_state
{
public:
    static const size_t npos;

    iterable_state(size_t i_pos = 0);
    iterable_state(const iterable_state&) = default;

    template<typename Action>
    inline void apply(const Action& i_action);
	template<typename ... Types>
	inline void apply(const variant<Types...>& i_action);
	size_t position() const;
    void reset();
    bool operator==(const iterable_state& other) const;
    bool operator!=(const iterable_state& other) const;

private:
	size_t m_currPos = 0;
};

}

#include "ddk_iterable_state.inl"
