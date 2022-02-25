
namespace ddk
{

template<typename T,typename TT>
const tagged_pointer_deleter& share_from_this<T,TT>::set_deleter(const tagged_pointer_deleter& i_deleter) const
{
	return (m_deleter = i_deleter);
}
template<typename T,typename TT>
void share_from_this<T,TT>::set_reference_counter(const tagged_reference_counter& i_refCounter) const
{
	m_refCounter = i_refCounter;
}
template<typename T,typename TT>
ddk::shared_reference_wrapper<TT> share_from_this<T,TT>::ref_from_this()
{
	if(m_refCounter->hasSharedReferences() == false)
	{
		throw bad_ptr_exception{ "Sharing non reference shareable object" };
	}

	return as_shared_reference(static_cast<TT*>(this),m_refCounter,m_deleter);
}
template<typename T,typename TT>
ddk::shared_reference_wrapper<const TT> share_from_this<T,TT>::ref_from_this() const
{
	if(m_refCounter->hasSharedReferences() == false)
	{
		throw bad_ptr_exception{ "Sharing non reference shareable object" };
	}

	return as_shared_reference(static_cast<const TT*>(this),m_refCounter,m_deleter);
}

}