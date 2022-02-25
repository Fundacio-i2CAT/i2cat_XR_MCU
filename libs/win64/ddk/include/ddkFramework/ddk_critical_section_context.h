#pragma once

#include "ddk_critical_section_defs.h"
#include "ddk_none.h"

namespace ddk
{

struct critical_section_context
{
public:
	critical_section_context(const detail::context_acquired_t& i_acq);
	critical_section_context(const critical_section_context&) = delete;
	critical_section_context(critical_section_context&& other);
	operator bool() const;

	critical_section_context& operator=(critical_section_context&& other);
	
private:

	bool m_acquired;
};

const detail::context_acquired_t context_acquired = detail::context_acquired_t(true);
const detail::context_acquired_t context_unacquired = detail::context_acquired_t(false);

}