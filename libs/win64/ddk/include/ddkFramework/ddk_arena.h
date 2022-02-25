#pragma once

#include <type_traits>
#include <cstddef>
#include "ddk_embedded_type.h"

namespace ddk
{

template<size_t ArenaSize, size_t ArenaAlignment>
class arena
{
	//in case we can, take benefit of 1 byte of alignment for boolean data
  static const size_t s_actualAlignment = ArenaAlignment;

public:
	arena();
	arena(const arena&) = delete;
	~arena();
	arena& operator=(const arena&) = delete;
	template<typename T, typename ... Args>
	inline void construct(Args&& ... i_args);
	template<typename T, typename ... Args>
	inline void assign(Args&& ... i_args);
	template<typename T, typename ... Args>
	inline void set_value(Args&& ... i_args);
	template<typename T>
	inline void destroy();
	template<typename T>
	inline typename embedded_type<T>::cref_type get() const;
	template<typename T>
	inline typename embedded_type<T>::ref_type get();
	template<typename T>
	inline embedded_type<T> extract();
	template<typename T>
	inline typename embedded_type<T>::cpointer_type get_ptr() const;
	template<typename T>
	inline typename embedded_type<T>::pointer_type get_ptr();
	inline char* get_arena();
	inline const char* get_arena() const;
	inline bool empty() const;
	template<typename T, typename Arg>
	inline bool compare(Arg&& i_arg) const;

private:
	typename std::aligned_storage<ArenaSize,s_actualAlignment>::type m_storage;
	bool m_empty;
};

template<typename T>
using typed_arena = arena<sizeof(embedded_type<T>),alignof(embedded_type<T>)>;

}

#include "ddk_arena.inl"
