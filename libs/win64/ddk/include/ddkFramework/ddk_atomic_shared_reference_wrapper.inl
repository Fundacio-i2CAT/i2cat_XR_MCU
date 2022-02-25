
namespace ddk
{
namespace detail
{

template<typename T, typename ReferenceCounter>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::atomic_shared_reference_wrapper_impl(const atomic_shared_reference_wrapper_impl& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T,typename ReferenceCounter>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::atomic_shared_reference_wrapper_impl(const shared_reference_wrapper_impl<T,ReferenceCounter>& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T,typename ReferenceCounter>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::atomic_shared_reference_wrapper_impl(atomic_shared_reference_wrapper_impl&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T,typename ReferenceCounter>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::atomic_shared_reference_wrapper_impl(shared_reference_wrapper_impl<T,ReferenceCounter>&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T,typename ReferenceCounter>
template<typename TT>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::atomic_shared_reference_wrapper_impl(const atomic_shared_reference_wrapper_impl<TT,ReferenceCounter>& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T,typename ReferenceCounter>
template<typename TT>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::atomic_shared_reference_wrapper_impl(const shared_reference_wrapper_impl<TT,ReferenceCounter>& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T,typename ReferenceCounter>
template<typename TT>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::atomic_shared_reference_wrapper_impl(atomic_shared_reference_wrapper_impl<TT,ReferenceCounter>&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T,typename ReferenceCounter>
template<typename TT>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::atomic_shared_reference_wrapper_impl(shared_reference_wrapper_impl<TT,ReferenceCounter>&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T,typename ReferenceCounter>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>& atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::operator=(const atomic_shared_reference_wrapper_impl<T,ReferenceCounter>& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(other);

	return *this;
}
template<typename T,typename ReferenceCounter>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>& atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::operator=(const shared_reference_wrapper_impl<T,ReferenceCounter>& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(other);

	return *this;
}
template<typename T,typename ReferenceCounter>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>& atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::operator=(atomic_shared_reference_wrapper_impl<T,ReferenceCounter>&& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(std::move(other));

	return *this;
}
template<typename T,typename ReferenceCounter>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>& atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::operator=(shared_reference_wrapper_impl<T,ReferenceCounter>&& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(std::move(other));

	return *this;
}
template<typename T,typename ReferenceCounter>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::operator shared_reference_wrapper_impl<T,ReferenceCounter>&()
{
	return promote_to_ref(atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::get());
}
template<typename T,typename ReferenceCounter>
atomic_shared_reference_wrapper_impl<T,ReferenceCounter>::operator const shared_reference_wrapper_impl<T,ReferenceCounter>&() const
{
	return promote_to_ref(atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::get());
}

}
}
