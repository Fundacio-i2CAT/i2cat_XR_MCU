#pragma once

namespace ddk
{

template<typename T>
inline T* get_raw_ptr(T* i_ref);
template<typename T>
inline void set_raw_ptr(T*& i_ref,T* i_value);
template<typename T>
inline T* extract_raw_ptr(T*& i_ref);
template<typename T>
inline void clear_ptr(T*& i_ref);

}

#include "ddk_pointer_defs.inl"
