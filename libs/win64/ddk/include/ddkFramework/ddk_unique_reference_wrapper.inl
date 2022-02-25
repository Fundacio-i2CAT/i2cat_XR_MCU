
namespace ddk
{

template<typename T>
unique_reference_wrapper<T>::unique_reference_wrapper(T* i_data,const tagged_pointer<unique_reference_counter>& i_refCounter,const tagged_pointer_deleter& i_refDeleter)
: unique_pointer_wrapper<T>(i_data,i_refCounter,i_refDeleter)
{
	DDK_ASSERT(i_data != NULL,"Trying to construct non null reference from void pointer");
}
template<typename T>
unique_reference_wrapper<T>::unique_reference_wrapper(T* i_data,tagged_pointer<unique_reference_counter>&& i_refCounter,const tagged_pointer_deleter& i_refDeleter)
: unique_pointer_wrapper<T>(i_data,std::move(i_refCounter),i_refDeleter)
{
	DDK_ASSERT(i_data != NULL,"Trying to construct non null reference from void pointer");
}
template<typename T>
unique_reference_wrapper<T>::unique_reference_wrapper(unique_reference_wrapper<T>&& other)
: unique_pointer_wrapper<T>(std::move(other))
{
}
template<typename T>
template<typename TT>
unique_reference_wrapper<T>::unique_reference_wrapper(unique_reference_wrapper<TT>&& other)
: unique_pointer_wrapper<T>(std::move(other))
{
}
template<typename T>
unique_reference_wrapper<T>& unique_reference_wrapper<T>::operator=(unique_reference_wrapper<T>&& other)
{
	unique_pointer_wrapper<T>::operator=(std::move(other));

	return *this;
}

}