
#include "ddk_macros.h"
#include "ddk_pointer_defs.h"

namespace ddk
{

template<typename T>
tagged_pointer<T>::tagged_pointer(const std::nullptr_t&)
: m_data(nullptr)
{
}
template<typename T>
tagged_pointer<T>::tagged_pointer(interface_ptr i_data)
: m_data(i_data)
{
}
template<typename T>
tagged_pointer<T>::tagged_pointer(interface_ptr i_data,short i_tag)
: m_data(nullptr)
{
	DDK_ASSERT(i_tag <= tag_mask,"You shall provide a tag less than tag mask");

	set_raw_ptr(m_data, as_pointer(as_number(i_data) | i_tag));
}
template<typename T>
tagged_pointer<T>::tagged_pointer(const tagged_pointer& other)
: m_data(other.m_data)
{
}
template<typename T>
tagged_pointer<T>::tagged_pointer(tagged_pointer&& other)
: m_data(nullptr)
{
	std::swap(m_data,other.m_data);
}
template<typename T>
template<typename TT>
tagged_pointer<T>::tagged_pointer(const tagged_pointer<TT>& other)
: m_data(other.m_data)
{
}
template<typename T>
template<typename TT>
tagged_pointer<T>::tagged_pointer(tagged_pointer<TT>&& other)
: m_data(nullptr)
{
	std::swap(m_data,other.m_data);
}
template<typename T>
tagged_pointer<T>::~tagged_pointer()
{
	m_data = nullptr;
}
template<typename T>
tagged_pointer<T>& tagged_pointer<T>::operator=(const tagged_pointer& other)
{
	m_data = other.m_data;

	return *this;
}
template<typename T>
template<typename TT>
tagged_pointer<T>& tagged_pointer<T>::operator=(const tagged_pointer<TT>& other)
{
	m_data = other.m_data;

	return *this;
}
template<typename T>
void tagged_pointer<T>::set_pointer(interface_ptr i_pointer)
{
	set_raw_ptr(m_data,as_pointer(as_number(i_pointer) | (as_number(m_data) & tag_mask)));
}
template<typename T>
void tagged_pointer<T>::set_tag(short i_tag)
{
	DDK_ASSERT(i_tag <= tag_mask,"You shall provide a tag less than tag mask");

	set_raw_ptr(m_data,as_pointer((as_number(m_data) & bitMask) | i_tag));
}
template<typename T>
short tagged_pointer<T>::get_tag() const
{
	return as_number(m_data) & tag_mask;
}
template<typename T>
bool tagged_pointer<T>::is_tagged() const
{
	return (as_number(m_data) & tag_mask) != 0;
}
template<typename T>
typename tagged_pointer<T>::pointer tagged_pointer<T>::operator->()
{
	return as_pointer(as_number(m_data) & bitMask);
}
template<typename T>
typename tagged_pointer<T>::const_pointer tagged_pointer<T>::operator->() const
{
	return as_pointer(as_number(m_data) & bitMask);
}
template<typename T>
typename tagged_pointer<T>::reference tagged_pointer<T>::operator*()
{
	if(pointer pData = as_pointer(as_number(m_data) & bitMask))
	{
		return *pData;
	}
	else
	{
		MAKE_IT_CRASH

		return *as_pointer(0xDEAD);
	}
}
template<typename T>
typename tagged_pointer<T>::const_reference tagged_pointer<T>::operator*() const
{
	if(pointer pData = as_pointer(as_number(m_data) & bitMask))
	{
		return *pData;
	}
	else
	{
		MAKE_IT_CRASH

		return *as_pointer(0xDEAD);
	}
}
template<typename T>
typename tagged_pointer<T>::pointer tagged_pointer<T>::get_pointer()
{
	return as_pointer(as_number(m_data) & bitMask);
}
template<typename T>
typename tagged_pointer<T>::const_pointer tagged_pointer<T>::get_pointer() const
{
	return as_pointer(as_number(m_data) & bitMask);
}
template<typename T>
typename tagged_pointer<T>::interface_ptr tagged_pointer<T>::get_data()
{
	return m_data;
}
template<typename T>
typename tagged_pointer<T>::interface_const_ptr tagged_pointer<T>::get_data() const
{
	return m_data;
}
template<typename T>
typename tagged_pointer<T>::pointer tagged_pointer<T>::extract_pointer()
{
	pointer res = as_pointer(as_number(m_data) & bitMask);

	m_data = nullptr;

	return res;
}
template<typename T>
tagged_pointer<T>::operator bool() const
{
	return m_data != nullptr;
}
template<typename T>
bool tagged_pointer<T>::operator==(const tagged_pointer& other) const
{
	return m_data == other.m_data;
}
template<typename T>
bool tagged_pointer<T>::operator!=(const tagged_pointer& other) const
{
	return m_data != other.m_data;
}
template<typename T>
typename tagged_pointer<T>::interface_ptr tagged_pointer<T>::tag_pointer(interface_ptr i_ptr,const short i_tag)
{
	return as_pointer(as_number(i_ptr) | i_tag);
}
template<typename T>
uintptr_t tagged_pointer<T>::tag_pointer_as_value(interface_ptr i_ptr,const short i_tag)
{
	return as_number(i_ptr) | i_tag;
}
template<typename T>
typename tagged_pointer<T>::interface_ptr tagged_pointer<T>::untag_pointer(interface_ptr i_ptr)
{
	return as_pointer(as_number(i_ptr) & bitMask);
}
template<typename T>
short tagged_pointer<T>::get_tag(interface_const_ptr i_ptr)
{
	return as_const_number(i_ptr) & tag_mask;
}
template<typename T>
bool tagged_pointer<T>::is_tagged(interface_const_ptr i_ptr)
{
	return (as_const_number(i_ptr) & tag_mask) != 0;
}
template<typename T>
typename tagged_pointer<T>::pointer tagged_pointer<T>::as_pointer(uintptr_t i_value)
{
	const from_number_to_ptr res = { i_value };

	return res.dataPointer;
}
template<typename T>
uintptr_t tagged_pointer<T>::as_number(interface_ptr i_ptr)
{
	const from_ptr_to_number res = { get_raw_ptr(i_ptr) };

	return res.dataNumber;
}
template<typename T>
uintptr_t tagged_pointer<T>::as_const_number(interface_const_ptr i_ptr)
{
	const from_const_ptr_to_number res = { get_raw_ptr(i_ptr) };

	return res.dataNumber;
}

template<typename T,typename ... Args>
tagged_pointer<T> make_tagged_pointer(short i_tag,Args&& ... i_args)
{
	return tagged_pointer<T>(new T(std::forward<Args>(i_args)...),i_tag);
}
template<typename TT,typename T>
tagged_pointer<TT> dynamic_tagged_cast(const tagged_pointer<T>& i_ptr)
{
	TT* pData = dynamic_cast<TT*>(i_ptr.get_pointer());

	return (pData) ? tagged_pointer<TT>(pData,i_ptr.get_tag()) : tagged_pointer<TT>();
}
template<typename TT,typename T>
tagged_pointer<TT> static_tagged_cast(const tagged_pointer<T>& i_ptr)
{
	return tagged_pointer<TT>(static_cast<TT*>(i_ptr.get_pointer()),i_ptr.get_tag());
}
template<typename TT,typename T>
tagged_pointer<TT> reinterpret_tagged_cast(const tagged_pointer<T>& i_ptr)
{
	return tagged_pointer<TT>(reinterpret_cast<TT*>(i_ptr.get_pointer()),i_ptr.get_tag());
}

}
