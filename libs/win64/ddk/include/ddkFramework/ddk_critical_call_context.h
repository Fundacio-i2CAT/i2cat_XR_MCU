#pragma once

#include "ddk_intrusive_node.h"
#include "ddk_lent_reference_wrapper.h"

template<typename AccessCallContext>
struct ConstCriticalCallContext;

template<typename AccessCallContext>
struct CriticalCallContext
{
	friend struct ConstCriticalCallContext<AccessCallContext>;

public:
	typedef typename AccessCallContext::provider_interface provider_interface;
	typedef typename AccessCallContext::critical_context critical_context;
	typedef ddk::lent_reference_wrapper<critical_context> critical_context_ref;
	typedef ddk::intrusive_node<critical_context_ref> critical_context_context;

	CriticalCallContext(AccessCallContext& i_callContext, critical_context_ref i_context)
	: m_callContext(i_callContext)
	, m_context(i_context)
	{
	}
	CriticalCallContext(const CriticalCallContext& other)
	: m_callContext(other.m_callContext)
	, m_context(other.m_context.get_value())
	{
	}
	~CriticalCallContext()
	{
		m_callContext.pop_call_context();
	}
	inline provider_interface* operator->()
	{
		m_callContext.push_call_context(ddk::lend(m_context));

		return &(m_callContext.get_provider());
	}
	inline const provider_interface* operator->() const
	{
		m_callContext.push_call_context(ddk::lend(m_context));

		return &(m_callContext.get_provider());
	}

private:
	AccessCallContext& m_callContext;
	mutable critical_context_context m_context;
};

template<typename AccessCallContext>
struct ConstCriticalCallContext
{
public:
	typedef typename AccessCallContext::provider_interface provider_interface;
	typedef typename AccessCallContext::critical_context critical_context;
	typedef ddk::lent_reference_wrapper<critical_context> critical_context_ref;
	typedef ddk::intrusive_node<critical_context_ref> critical_context_context;

	ConstCriticalCallContext(const AccessCallContext& i_callContext, critical_context_ref i_context)
	: m_callContext(i_callContext)
	, m_context(i_context)
	{
	}
	ConstCriticalCallContext(const ConstCriticalCallContext& other)
	: m_callContext(other.m_callContext)
	, m_context(other.m_context.get_value())
	{
	}
	ConstCriticalCallContext(const CriticalCallContext<AccessCallContext>& other)
	: m_callContext(other.m_callContext)
	, m_context(other.m_context.get_value())
	{
	}
	~ConstCriticalCallContext()
	{
		m_callContext.pop_call_context();
	}
	inline const provider_interface* operator->() const
	{
		m_callContext.push_call_context(ddk::lend(m_context));

		return &(m_callContext.get_provider());
	}

private:
	const AccessCallContext& m_callContext;
	mutable critical_context_context m_context;
};