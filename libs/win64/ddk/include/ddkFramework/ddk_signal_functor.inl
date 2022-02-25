
namespace ddk
{

template<typename ... Types>
signal_functor<void(Types...)>::signal_functor(const ddk::function<void(Types...)>& call,const detail::signal_connector& i_connector)
: connection_base(i_connector)
, m_call(call)
{
}
template<typename ... Types>
template<typename ... Args>
void signal_functor<void(Types...)>::execute(Args&& ... i_args) const
{
	eval(m_call,std::forward<Args>(i_args) ...);
}
template<typename ... Types>
void signal_functor<void(Types...)>::execute_tuple(const tuple_t& i_args) const
{
	typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>()>::type range_seq;

	_execute(i_args,range_seq{});
}
template<typename ... Types>
bool signal_functor<void(Types...)>::operator==(const signal_functor& other) const
{
	return get_id() == other.get_id();
}
template<typename ... Types>
template<size_t ... Indexs>
inline void signal_functor<void(Types...)>::_execute(const tuple_t& i_args,const mpl::sequence<Indexs...>&) const
{
	eval(m_call,std::get<Indexs>(i_args) ...);
}

}
