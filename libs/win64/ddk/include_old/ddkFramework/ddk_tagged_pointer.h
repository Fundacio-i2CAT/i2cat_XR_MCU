#pragma once

#include "ddk_template_helper.h"
#include <type_traits>
#include <cstring>

#define TAG_POINTER(_PTR,_TAG) reinterpret_cast<size_t&>(*(&_PTR)) |= _TAG;
#define TAGGED_POINTER(_PTR,_TAG) (reinterpret_cast<const size_t&>(*(&_PTR)) & _TAG)

#define MASK_NUM_BITS 2

namespace ddk
{

template<typename T>
class tagged_pointer
{
	static const short tag_mask = (1 << MASK_NUM_BITS) - 1;
	static const uintptr_t bitMask = ~tag_mask;

	template<typename TT>
	friend class tagged_pointer;
	template<typename TTT, typename TT>
	friend tagged_pointer<TTT> dynamic_tagged_cast(const tagged_pointer<TT>&);
	template<typename TTT, typename TT>
	friend tagged_pointer<TTT> static_tagged_cast(const tagged_pointer<TT>&);
	template<typename TTT, typename TT>
	friend tagged_pointer<TTT> reinterpret_tagged_cast(const tagged_pointer<TT>&);

public:
	typedef typename mpl::get_pointer<T>::type interface_ptr;
	typedef typename mpl::get_pointer<T>::const_type interface_const_ptr;
	typedef typename mpl::get_pointer<T>::pointer pointer;
	typedef typename mpl::get_pointer<T>::const_pointer const_pointer;
	typedef typename mpl::get_pointer<T>::reference reference;
	typedef typename mpl::get_pointer<T>::const_reference const_reference;

	tagged_pointer() = default;
	tagged_pointer(const std::nullptr_t&);
	tagged_pointer(interface_ptr i_data);
	tagged_pointer(interface_ptr i_data, short i_tag);
	inline tagged_pointer(const tagged_pointer& other);
	inline tagged_pointer(tagged_pointer&& other);
	template<typename TT>
	tagged_pointer(const tagged_pointer<TT>& other);
	template<typename TT>
	tagged_pointer(tagged_pointer<TT>&& other);
	~tagged_pointer();
	tagged_pointer& operator=(const tagged_pointer& other);
	template<typename TT>
	tagged_pointer& operator=(const tagged_pointer<TT>& other);
	inline void set_pointer(interface_ptr i_pointer);
	inline void set_tag(short i_tag);
	inline short get_tag() const;
	inline bool is_tagged() const;
	inline pointer operator->();
	inline const_pointer operator->() const;
	inline reference operator*();
	inline const_reference operator*() const;
	inline pointer get_pointer();
	inline const_pointer get_pointer() const;
	inline interface_ptr get_data();
	inline interface_const_ptr get_data() const;
	inline pointer extract_pointer();
	inline operator bool() const;
	inline bool operator==(const tagged_pointer& other) const;
	inline bool operator!=(const tagged_pointer& other) const;
	static inline interface_ptr tag_pointer(interface_ptr i_ptr, const short i_tag);
	static inline uintptr_t tag_pointer_as_value(interface_ptr i_ptr, const short i_tag);
	static inline interface_ptr untag_pointer(interface_ptr i_ptr);
	static inline short get_tag(interface_const_ptr i_ptr);
	static inline bool is_tagged(interface_const_ptr i_ptr);

private:
    union from_number_to_ptr
    {
        uintptr_t dataNumber;
		pointer dataPointer;
    };
    union from_ptr_to_number
    {
		pointer dataPointer;
        uintptr_t dataNumber;
    };
	union from_const_ptr_to_number
	{
		const_pointer dataPointer;
		uintptr_t dataNumber;
	};

	static inline pointer as_pointer(uintptr_t i_value);
	static inline uintptr_t as_number(interface_ptr i_ptr);
	static inline uintptr_t as_const_number(interface_const_ptr i_ptr);

	mutable interface_ptr m_data = nullptr;
};

template<typename T, typename ... Args>
inline tagged_pointer<T> make_tagged_pointer(short i_tag, Args&& ... i_args);
template<typename TT, typename T>
inline tagged_pointer<TT> dynamic_tagged_cast(const tagged_pointer<T>& i_ptr);
template<typename TT, typename T>
inline tagged_pointer<TT> static_tagged_cast(const tagged_pointer<T>& i_ptr);
template<typename TT, typename T>
inline tagged_pointer<TT> reinterpret_tagged_cast(const tagged_pointer<T>& i_ptr);

}

#include "ddk_tagged_pointer.inl"
