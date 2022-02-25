#pragma once

#include <string>

namespace ddk
{
namespace rtti
{

namespace detail
{

struct rtti_tag_t;

}

struct TypeInfo
{
	template<typename,typename>
	friend TypeInfo make_type_info();
	template<typename>
	friend TypeInfo make_type_info();
	friend inline size_t hash(const TypeInfo& i_typeInfo)
	{
		return i_typeInfo.m_id;	
	}

public:
	static const size_t s_invalid = -1;

	constexpr TypeInfo() = default;
	constexpr TypeInfo(size_t i_id);
	size_t constexpr get_id() const;
	constexpr bool operator==(const TypeInfo& other) const;
	constexpr bool empty() const;

private:
	size_t m_id = s_invalid;
};

template<typename T, typename TT>
inline TypeInfo make_type_info();

template<typename T>
inline TypeInfo make_type_info();

template<typename T>
inline const TypeInfo& type_info();

template<typename T, typename TT>
inline const TypeInfo& type_info();

}
}

#include "ddk_rtti_defs.inl"
