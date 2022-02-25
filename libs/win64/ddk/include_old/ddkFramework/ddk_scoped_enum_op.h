#pragma once

#include "ddk_flagset.h"

namespace ddk
{
namespace detail
{

template<typename Enum>
struct scoped_enum_op_or
{
public:
	scoped_enum_op_or(const Enum& i_lhs, const Enum& i_rhs)
	: m_lhs(i_lhs)
	, m_rhs(i_rhs)
	{
	}
	operator int() const
	{
		return m_lhs.getValue() | m_rhs.getValue();
	}
	operator ddk::flag_set<Enum>() const
	{
		ddk::flag_set<Enum> res(m_lhs);

		res = res | m_rhs;

		return res;
	}

private:
	Enum m_lhs;
	Enum m_rhs;
};

template<typename Enum>
struct scoped_enum_op_and
{
public:
	scoped_enum_op_and(const Enum& i_lhs, const Enum& i_rhs)
	: m_lhs(i_lhs)
	, m_rhs(i_rhs)
	{
	}
	operator int() const
	{
		return m_lhs.getValue() & m_rhs.getValue();
	}
	operator ddk::flag_set<Enum>() const
	{
		ddk::flag_set<Enum> res(m_lhs);

		res = res & m_rhs;
	}

private:
	Enum m_lhs;
	Enum m_rhs;
};

}
}