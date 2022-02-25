#pragma once

#include "ddk_scoped_enum.h"
#include "ddk_type_id.h"
#include "ddk_signal.h"

namespace ddk
{

class IAccessProvider
{
public:
	enum Access
	{
		READING = 0,
		WRITING,
		NONE
	};
};

struct critical_access_identifier_t;
typedef Id<size_t,critical_access_identifier_t> critical_section_identifier;
const critical_section_identifier k_invalidCriticalAccessIdentifier = critical_section_identifier(0);

SCOPED_ENUM_DECL(Reentrancy,
		REENTRANT,
		NON_REENTRANT
	);

namespace detail
{

struct context_acquired_t
{
	context_acquired_t(bool i_acquired)
	: m_acquired(i_acquired)
	{}

	bool m_acquired;
};

}
}