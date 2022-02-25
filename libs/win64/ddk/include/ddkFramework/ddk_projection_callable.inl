
namespace ddk
{

template<size_t Component>
TEMPLATE(typename ... Args)
REQUIRED(IS_NUMBER_OF_ARGS_GREATER(Component,Args...))
mpl::nth_type_of_t<Component,Args...> projection_callable<Component>::operator()(Args&& ... i_args) const
{
    typedef typename std::remove_reference<mpl::nth_type_of_t<Component,Args...>>::type return_type;

    const void* _[] = { &i_args ...};

    return *reinterpret_cast<return_type*>(const_cast<void*>(_[Component]));
}

}
