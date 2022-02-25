#pragma once

#include "ddk_template_helper.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<size_t Component>
struct projection_callable
{
public:
    struct callable_tag;

    projection_callable() = default;

    TEMPLATE(typename ... Args)
    REQUIRES(IS_NUMBER_OF_ARGS_GREATER(Component,Args...))
    inline mpl::nth_type_of_t<Component,Args...> operator()(Args&& ... i_args) const;
};

template<size_t Component>
const projection_callable<Component> projection = projection_callable<Component>{};

}

#include "ddk_projection_callable.inl"