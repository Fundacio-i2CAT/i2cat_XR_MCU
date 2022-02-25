
namespace ddk
{
namespace detail
{

template<typename T,typename ReferenceCounter>
shared_reference_wrapper_impl<T,ReferenceCounter>::shared_reference_wrapper_impl(T* i_data,const tagged_pointer<ReferenceCounter>& i_refCounter,const tagged_pointer_deleter& i_refDeleter)
: shared_pointer_wrapper_impl<T,ReferenceCounter>(i_data,i_refCounter,i_refDeleter)
{
	DDK_ASSERT(i_data != NULL,"Trying to construct non null reference from void pointer");
}
template<typename T,typename ReferenceCounter>
shared_reference_wrapper_impl<T,ReferenceCounter>::shared_reference_wrapper_impl(const shared_reference_wrapper_impl& other)
: shared_pointer_wrapper_impl<T,ReferenceCounter>(other)
{
}
template<typename T,typename ReferenceCounter>
shared_reference_wrapper_impl<T,ReferenceCounter>::shared_reference_wrapper_impl(shared_reference_wrapper_impl&& other)
: shared_pointer_wrapper_impl<T,ReferenceCounter>(std::move(other))
{
}
template<typename T,typename ReferenceCounter>
template<typename TT>
shared_reference_wrapper_impl<T,ReferenceCounter>::shared_reference_wrapper_impl(const shared_reference_wrapper_impl<TT,ReferenceCounter>& other)
: shared_pointer_wrapper_impl<T,ReferenceCounter>(other)
{
}
template<typename T,typename ReferenceCounter>
template<typename TT>
shared_reference_wrapper_impl<T,ReferenceCounter>::shared_reference_wrapper_impl(shared_reference_wrapper_impl<TT,ReferenceCounter>&& other)
: shared_pointer_wrapper_impl<T,ReferenceCounter>(std::move(other))
{
}
template<typename T,typename ReferenceCounter>
shared_reference_wrapper_impl<T,ReferenceCounter>& shared_reference_wrapper_impl<T,ReferenceCounter>::operator=(const shared_reference_wrapper_impl<T,ReferenceCounter>& other)
{
	shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(other);

	return *this;
}
template<typename T,typename ReferenceCounter>
shared_reference_wrapper_impl<T,ReferenceCounter>& shared_reference_wrapper_impl<T,ReferenceCounter>::operator=(shared_reference_wrapper_impl<T,ReferenceCounter>&& other)
{
	shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(std::move(other));

	return *this;
}

}
}