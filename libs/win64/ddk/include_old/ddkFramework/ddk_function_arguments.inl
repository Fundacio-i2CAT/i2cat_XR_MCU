
namespace ddk
{

template<typename Arg,typename ... Args>
function_arguments<Arg,Args...> make_function_arguments(Arg&& i_arg, Args&& ... i_args)
{
	return { std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...};
}

}