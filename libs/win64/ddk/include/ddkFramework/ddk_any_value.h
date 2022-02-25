#pragma once

#include "ddk_macros.h"
#include "ddk_any_value_wrapper.h"
#include "ddk_type_id.h"
#include "ddk_tagged_pointer.h"
#include "ddk_crash.h"
#include "ddk_container_exceptions.h"

namespace ddk
{
namespace detail
{

inline size_t __getTypeId__()
{
	static size_t s_typeId = 0;

	return s_typeId++;
}

}

template<typename T>
inline size_t getTypeId()
{
	static const size_t s_typeId = detail::__getTypeId__();

	return s_typeId;
}

struct any_value
{
typedef value_wrapper_interface* InterfacePtr;

public:
	static const size_t k_invalidType = -1;

	any_value();
	any_value(const any_value& other);
	template<typename T>
	any_value(const T& i_val, typename std::enable_if<std::is_same<T,any_value>::value == false>::type* = NULL);
	~any_value();
	any_value& operator=(const any_value& other);
	template<typename T>
	inline bool isOfType() const;
	template<typename T, typename TT>
	inline void setValue(TT&& i_value);
	template<typename T>
	inline typename value_wrapper<T>::lreference getValue();
	template<typename T>
	inline typename value_wrapper<T>::const_lreference getValue() const;
	template<typename T>
	inline typename value_wrapper<T>::raw_type extractValue();
	operator bool() const;
	size_t getCurrTypeId() const;
	void clear();

private:
	void destroy();
	void clone(const any_value& other);
	virtual size_t getArenaSize() const;

	size_t m_currentAgnosticType;
	tagged_pointer<value_wrapper_interface> m_arena;
};

}

#include "ddk_any_value.inl"
