#pragma once

#include <sstream>
#include "ddk_formatter.h"

namespace ddk
{

template<typename UnderlyingType, typename T>
struct Id
{
	friend inline std::string& operator<<(std::string& o_str, const Id<UnderlyingType,T>& i_value)
	{
		return o_str << i_value.m_key;
	}
	friend inline std::ostringstream& operator<<(std::ostringstream& o_str, const Id<UnderlyingType,T>& i_value)
	{
		o_str << i_value.m_key;

		return o_str;
	}
	friend inline Id<UnderlyingType,T> operator+(const Id<UnderlyingType,T>& i_lhs, const Id<UnderlyingType,T>& i_rhs)
	{
		return Id<UnderlyingType,T>(i_lhs.m_key + i_rhs.m_key);
	}

public:
	typedef UnderlyingType underlying_type;

	explicit Id()
	: m_key(UnderlyingType())
	{}
	explicit Id(const UnderlyingType& i_key)
	: m_key(i_key)
	{}
	explicit Id(const UnderlyingType& i_key, const std::string& i_name)
	: m_key(i_key)
	, m_name(i_name)
	{}
	inline bool operator==(const Id& other) const
	{
		return m_key == other.m_key;
	}
	inline bool operator!=(const Id& other) const
	{
		return m_key != other.m_key;
	}
	inline bool operator==(const UnderlyingType& other) const
	{
		return m_key == other;
	}
	inline bool operator!=(const UnderlyingType& other) const
	{
		return m_key != other;
	}
	inline bool operator<(const Id& other) const
	{
		return m_key < other.m_key;
	}
	inline bool operator<=(const Id& other) const
	{
		return m_key <= other.m_key;
	}
	inline bool operator>(const Id& other) const
	{
		return m_key > other.m_key;
	}
	inline bool operator>=(const Id& other) const
	{
		return m_key >= other.m_key;
	}
	inline UnderlyingType* operator->()
	{
		return &m_key;
	}
	inline const UnderlyingType* operator->() const
	{
		return m_key;
	}
	inline UnderlyingType getValue() const
	{
		return m_key;
	}
	inline std::string name() const
	{
		return m_name;
	}

private:
	UnderlyingType m_key;
	std::string m_name;
};

}

namespace std
{

template<typename UnderlyingType, typename T>
struct hash<ddk::Id<UnderlyingType,T>>
{
    std::size_t operator()(const ddk::Id<UnderlyingType,T> s) const noexcept
    {
        return std::hash<UnderlyingType>{}(s.getValue());
    }
};

}

namespace ddk
{

template<typename UnderlyingType, typename T>
struct formatter<Id<UnderlyingType,T>>
{
static Id<UnderlyingType,T> format(const std::string& i_str)
{
	return Id<UnderlyingType,T>(formatter<UnderlyingType>::format(i_str));
}
};

}