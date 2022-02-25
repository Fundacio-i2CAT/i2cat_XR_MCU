#pragma once

#include "ddk_scoped_enum_op.h"
#include "ddk_scoped_enum_tag.h"
#include <string>

template<typename T>
inline const char* get_enum_name(const T&)
{
	static_assert(sizeof(T)==0, "You shall provide names macro SCOPED_ENUM_DECL_NAMES defined below for this type");

	return NULL;
}
template<typename T>
inline bool get_enum_from_name(T& className, const char* i_name)
{
	static_assert(sizeof(T)==0, "You shall provide names macro SCOPED_ENUM_DECL_NAMES defined below for this type");

    return false;
}

#define SCOPED_ENUM_DECL(ClassName, ...) \
struct ClassName \
{ \
	enum __val__\
	{ \
		__VA_ARGS__, \
		Count \
	}; \
	friend inline ddk::flag_set<ClassName> operator|(const ddk::flag_set<ClassName>& i_flags, __val__ i_value) \
	{ \
		ddk::flag_set<ClassName> res(i_flags); \
		res |= ClassName(i_value); \
		return res; \
	} \
	friend inline ddk::detail::scoped_enum_op_or<ClassName> operator|(__val__ i_lhs, __val__ i_rhs) \
	{ \
		return ddk::detail::scoped_enum_op_or<ClassName>(i_lhs,i_rhs); \
	} \
	friend inline ddk::detail::scoped_enum_op_and<ClassName> operator&(__val__ i_lhs, __val__ i_rhs) \
	{ \
		return ddk::detail::scoped_enum_op_and<ClassName>(i_lhs,i_rhs); \
	} \
	friend inline ddk::flag_set<ClassName>& operator|=(ddk::flag_set<ClassName>& i_flags, __val__ i_value) \
	{ \
		i_flags |= ClassName(i_value); \
		return i_flags; \
	} \
	friend inline ddk::flag_set<ClassName> operator&(const ddk::flag_set<ClassName>& i_flags, __val__ i_value) \
	{ \
		ddk::flag_set<ClassName> res(i_flags); \
		res &= ClassName(i_value); \
		return res; \
	} \
	friend inline ddk::flag_set<ClassName>& operator&=(ddk::flag_set<ClassName>& i_flags, __val__ i_value) \
	{ \
		i_flags &= ClassName(i_value); \
		return i_flags; \
	} \
public: \
	typedef ddk::detail::scoped_enum_tag scoped_enum_tag; \
	typedef __val__ underlying_type; \
	\
	explicit ClassName() \
	: internalVal(static_cast<__val__>(0)) \
	{} \
	explicit ClassName(int i_val) \
	: internalVal(static_cast<__val__>(i_val)) \
	{} \
	ClassName(const ClassName::__val__& i_val) \
	: internalVal(i_val) \
	{} \
	inline bool operator==(const ClassName& other) const \
	{ \
		return internalVal == other.internalVal; \
	} \
	inline bool operator!=(const ClassName& other) const \
	{ \
		return internalVal != other.internalVal; \
	} \
	inline bool operator==(const ClassName::__val__& i_val) const \
	{ \
		return internalVal == i_val; \
	} \
	inline bool operator!=(const ClassName::__val__& i_val) const \
	{ \
		return internalVal != i_val; \
	} \
	inline bool operator<(const ClassName& other) const \
	{ \
		return internalVal < other.internalVal; \
	} \
	inline operator underlying_type() const \
	{ \
		return internalVal; \
	} \
	inline int getValue() const \
	{ \
		return static_cast<int>(internalVal); \
	} \
private: \
	\
	__val__ internalVal; \
};


#define SCOPED_ENUM_DECL_NAMES(ClassName,...) \
inline const char* get_enum_name(const ClassName& i_enum) \
{ \
	static const char* _names[ClassName::Count] = { __VA_ARGS__ }; \
	\
	return _names[i_enum.getValue()]; \
} \
inline const char* get_enum_name(const ClassName::underlying_type& i_enum) \
{ \
	static const char* _names[ClassName::Count] = { __VA_ARGS__ }; \
	\
	return _names[i_enum]; \
} \
inline bool get_enum_from_name(ClassName& className, const char* i_name) \
{ \
	static const char* _names[ClassName::Count] = { __VA_ARGS__ }; \
	for(int nameIndex=0;nameIndex<ClassName::Count;++nameIndex) \
	{ \
		if(strcmp(_names[nameIndex],i_name) == 0) \
		{ \
			className = ClassName(nameIndex); \
			\
			return true; \
		} \
	} \
	\
	return false; \
}
