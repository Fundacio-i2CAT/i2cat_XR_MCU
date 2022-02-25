
namespace ddk
{
namespace detail
{

template<typename T>
iterable_order<T>::iterable_order(const T& i_order)
: m_order(i_order)
{
}
template<typename T>
const T& iterable_order<T>::get_order() const
{
	return m_order;
}

template<typename T, typename Traits>
ordered_iterable_impl<T,Traits>::ordered_iterable_impl(iterable_impl_dist_ref<iterable_base_traits> i_iterableRef, const T& i_actionResolver)
: m_iterableRef(i_iterableRef)
, m_actionResolver(i_actionResolver)
{
}
template<typename T, typename Traits>
void ordered_iterable_impl<T,Traits>::iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
	m_iterableRef->iterate_impl(make_function([i_try,this](reference i_value) -> action { return m_actionResolver.resolve(eval(i_try,i_value)); }),m_actionResolver.resolve(i_initialAction),i_actionStatePtr);
}
template<typename T, typename Traits>
void ordered_iterable_impl<T,Traits>::iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const
{
	m_iterableRef->iterate_impl(make_function([i_try,this](const_reference i_value) -> action { return m_actionResolver.resolve(eval(i_try,i_value)); }),m_actionResolver.resolve(i_initialAction),i_actionStatePtr);
}
template<typename T, typename Traits>
size_t ordered_iterable_impl<T,Traits>::size() const
{
	return m_iterableRef->size();
}
template<typename T, typename Traits>
bool ordered_iterable_impl<T,Traits>::empty() const
{
	return m_iterableRef->empty();
}

}
}
