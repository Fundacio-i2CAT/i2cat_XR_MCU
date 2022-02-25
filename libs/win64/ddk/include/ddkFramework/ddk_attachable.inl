
namespace ddk
{

template<typename Return>
attachable<Return>::attachable(cancellable_executor_unique_ref<Return> i_executorImpl)
: m_executorImpl(std::move(i_executorImpl))
{
}

template<typename T>
attachable<void> attach(thread i_thread)
{
	return attachable<T>(make_executor<detail::thread_executor<T>>(std::move(i_thread)));
}
template<typename T>
attachable<void> attach(fiber i_fiber)
{
	return attachable<T>(make_executor<detail::fiber_executor<T>>(std::move(i_fiber)));
}
template<typename T>
attachable<void> attach(cancellable_executor_unique_ref<detail::void_t> i_attachable)
{
	return attachable<T>(std::move(i_attachable));
}

}