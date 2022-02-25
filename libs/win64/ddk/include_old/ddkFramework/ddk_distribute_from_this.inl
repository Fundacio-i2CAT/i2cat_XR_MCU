
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename T,typename TT>
distribute_from_this<T,TT>::distribute_from_this(const tagged_pointer_deleter& i_deleter)
: m_deleter(i_deleter)
{
}
template<typename T,typename TT>
ddk::distributed_reference_wrapper<TT> distribute_from_this<T,TT>::ref_from_this()
{
	return as_shared_reference(static_cast<TT*>(this),tagged_pointer<distributed_reference_counter>{ &m_refCounter, ReferenceAllocationType::Embedded },m_deleter);
}
template<typename T,typename TT>
ddk::distributed_reference_wrapper<const TT> distribute_from_this<T,TT>::ref_from_this() const
{
	return as_shared_reference(static_cast<const TT*>(this),tagged_pointer<distributed_reference_counter>{ &m_refCounter, ReferenceAllocationType::Embedded },m_deleter);
}
template<typename T,typename TT>
typename distribute_from_this<T,TT>::tagged_reference_counter distribute_from_this<T,TT>::get_reference_counter() const
{
	return { &m_refCounter, ReferenceAllocationType::Embedded };
}
template<typename T,typename TT>
tagged_pointer_deleter distribute_from_this<T,TT>::get_deleter() const
{
	return m_deleter;
}
template<typename T,typename TT>
void distribute_from_this<T,TT>::set_deleter(const tagged_pointer_deleter& i_deleter) const
{
	m_deleter = i_deleter;
}

}