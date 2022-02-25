
#include "ddk_function_exceptions.h"
#include "ddk_allocator_exceptions.h"

namespace ddk
{
namespace detail
{

const size_t k_small_buffer_allocation_size = 40;

template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl>
function_impl<Return(Types...),Allocator,FunctionImpl>::function_impl()
: m_allocator(k_small_buffer_allocation_size,Allocator())
{
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
template<typename AAllocator>
function_impl<Return(Types...),Allocator,FunctionImpl>::function_impl(const function_impl<Return(Types...),AAllocator,FunctionImpl>& other)
: m_functionImpl(other.m_functionImpl)
, m_allocator(other.m_allocator)
{
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
template<typename AAllocator>
function_impl<Return(Types...),Allocator,FunctionImpl>::function_impl(function_impl<Return(Types...),AAllocator,FunctionImpl>&& other)
: m_functionImpl(std::move(other.m_functionImpl))
, m_allocator(other.m_allocator)
{
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
function_impl<Return(Types...),Allocator,FunctionImpl>::function_impl(std::nullptr_t)
: m_allocator(k_small_buffer_allocation_size,Allocator())
{
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
TEMPLATE(typename T)
REQUIRED(IS_CALLABLE_NOT_FUNCTION(T,Types...))
function_impl<Return(Types...),Allocator,FunctionImpl>::function_impl(T&& i_functor, const Allocator& i_allocator)
: m_allocator(k_small_buffer_allocation_size,i_allocator)
{
    typedef typename mpl::static_if<detail::is_base_of_inherited_functor_impl<mpl::remove_qualifiers<T>,Return,Types...>,mpl::remove_qualifiers<T>,detail::aggregated_functor_impl<mpl::remove_qualifiers<T>,Return,Types...>>::type Functor;

	std::pair<resource_deleter_const_lent_ref,void*> allocCtxt = m_allocator.allocate(sizeof(Functor));

	Functor* newFuncImpl = new(allocCtxt.second) Functor(std::forward<T>(i_functor));

    m_functionImpl = as_distributed_reference(newFuncImpl,allocCtxt.first);
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
function_impl<Return(Types...),Allocator,FunctionImpl>::function_impl(Return(*i_call)(Types...), const Allocator& i_allocator)
: m_allocator(k_small_buffer_allocation_size,i_allocator)
{
    typedef detail::free_function_impl<Return,Types...> Functor;

	std::pair<resource_deleter_const_lent_ref,void*> allocCtxt = m_allocator.allocate(sizeof(Functor));

    Functor* newFuncImpl = new(allocCtxt.second) Functor(i_call);

    m_functionImpl = as_distributed_reference(newFuncImpl,allocCtxt.first);
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
template<typename T>
function_impl<Return(Types...),Allocator,FunctionImpl>::function_impl(T *i_pRef, Return(T::*i_call)(Types...), const Allocator& i_allocator)
: m_allocator(k_small_buffer_allocation_size,i_allocator)
{
    typedef detail::relative_function_impl<T,Return,Types...> Functor;

	std::pair<resource_deleter_const_lent_ref,void*> allocCtxt = m_allocator.allocate(sizeof(Functor));

	Functor* newFuncImpl = new(allocCtxt.second) Functor(i_pRef,i_call);

	m_functionImpl = as_distributed_reference(newFuncImpl,allocCtxt.first);
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
template<typename T>
function_impl<Return(Types...),Allocator,FunctionImpl>::function_impl(const T *i_pRef, Return(T::*i_call)(Types...)const, const Allocator& i_allocator)
: m_allocator(k_small_buffer_allocation_size,i_allocator)
{
    typedef detail::relative_function_impl<const T,Return,Types...> Functor;

	std::pair<resource_deleter_const_lent_ref,void*> allocCtxt = m_allocator.allocate(sizeof(Functor));

	Functor* newFuncImpl = new(allocCtxt.second) Functor(i_pRef,i_call);

	m_functionImpl = as_distributed_reference(newFuncImpl,allocCtxt.first);
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
function_impl<Return(Types...),Allocator,FunctionImpl>& function_impl<Return(Types...),Allocator,FunctionImpl>::operator=(std::nullptr_t)
{
    m_functionImpl = nullptr;

    return *this;
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
bool function_impl<Return(Types...),Allocator,FunctionImpl>::operator==(std::nullptr_t) const
{
    return m_functionImpl.empty();
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
bool function_impl<Return(Types...),Allocator,FunctionImpl>::operator!=(std::nullptr_t) const
{
    return m_functionImpl.empty() == false;
}
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Types,Args)...)
Return function_impl<Return(Types...),Allocator,FunctionImpl>::inline_eval(Args&& ... i_args) const
{
	if constexpr (std::is_same<Return,void>::value)
	{
		m_functionImpl->operator()(std::forward<Args>(i_args) ...);
	}
	else
	{
		return m_functionImpl->operator()(std::forward<Args>(i_args) ...);
	}
}
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Types,Args)...)
Return function_impl<Return(Types...),Allocator,FunctionImpl>::inline_eval(const function_arguments<Args...>& i_args) const
{
    return eval_arguments(typename mpl::make_sequence<0,mpl::get_num_types<Args...>()>::type{},i_args);
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
template<typename ... Args>
NO_DISCARD_RETURN resolved_function<Return,detail::unresolved_types<mpl::type_pack<Args...>,Types...>,Allocator> function_impl<Return(Types...),Allocator,FunctionImpl>::operator()(Args&& ... i_args) const
{
    if(m_functionImpl)
    {
        resolved_function<Return,detail::unresolved_types<mpl::type_pack<Args...>,Types...>,Allocator> res;

        res.m_functionImpl = m_functionImpl->specialize(m_allocator,std::forward<Args>(i_args) ...);

        return res;
    }
    else
    {
        throw call_function_exception{"Trying to call empty function_impl"};
    }
}
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
template<size_t ... Indexs, typename ... Args>
Return function_impl<Return(Types...),Allocator,FunctionImpl>::eval_arguments(const mpl::sequence<Indexs...>&, const function_arguments<Args...>& i_args) const
{
    if(m_functionImpl)
    {
        if constexpr (std::is_same<Return,void>::value)
        {
            m_functionImpl->operator()(i_args.template get<Indexs>() ...);
        }
        else
        {
            return m_functionImpl->operator()(i_args.template get<Indexs>() ...);
        }
    }
    else
    {
        throw call_function_exception{"Trying to call empty function_impl"};
    }
}

}

template<typename Return,typename ... Types,typename Allocator>
function<Return(Types...),Allocator>& function<Return(Types...),Allocator>::operator=(std::nullptr_t)
{
    function_base_t::operator=(nullptr);

    return *this;
}

}
