#pragma once

#include "ddk_thread_utils.h"
#include "ddk_critical_call_context.h"
#include "ddk_critical_section_context.h"
#include "ddk_intrusive_stack.h"
#include "ddk_intrusive_node.h"
#include "ddk_thread_local.h"

namespace ddk
{
namespace detail
{

template<typename,bool>
class IAccessCriticalCallContextImpl;

template<typename Traits>
class IAccessCriticalCallContextImpl<Traits,false>: protected Traits::provider_interface
{
	typedef ddk::lent_reference_wrapper<typename Traits::critical_context> critical_context_ref;
	typedef ddk::lent_reference_wrapper<const typename Traits::critical_context> critical_context_const_ref;
	typedef ddk::intrusive_stack<critical_context_ref> context_intrusive_stack;

public:
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef ddk::intrusive_node<critical_context_ref> context_node_base;
	typedef CriticalCallContext<IAccessCriticalCallContextImpl<Traits,false>> critical_call_context;
	typedef ConstCriticalCallContext<IAccessCriticalCallContextImpl<Traits,false>> const_critical_call_context;

	inline const_critical_call_context create_call_context(const critical_context_ref& i_context) const
	{
		return const_critical_call_context(*this,i_context);
	}
	inline critical_call_context create_call_context(const critical_context_ref& i_context)
	{
		return critical_call_context(*this,i_context);
	}
	inline critical_context& acquire_call_context()
	{
		return *(get_context_stack().get_front_value());
	}
	inline const critical_context& acquire_call_context() const
	{
		return *(get_context_stack().get_front_value());
	}
	inline provider_interface& get_provider()
	{
		return *static_cast<provider_interface*>(this);
	}
	inline const provider_interface& get_provider() const
	{
		return *static_cast<const provider_interface*>(this);
	}
	inline void push_call_context(ddk::lent_reference_wrapper<context_node_base> i_node) const
	{
		get_context_stack().push(std::move(i_node));
	}
	inline void pop_call_context() const
	{
		get_context_stack().pop();
	}

private:
	static context_intrusive_stack& get_context_stack()
	{
		static ddk::threadlocal<context_intrusive_stack,Traits> s_contextStack;

		return s_contextStack.acquire(context_intrusive_stack::Lifo);
	}
};

template<typename Traits>
class IAccessCriticalCallContextImpl<Traits,true>: protected Traits::provider_interface
{
	typedef ddk::lent_reference_wrapper<typename Traits::critical_context> critical_context_ref;
	typedef ddk::lent_reference_wrapper<const typename Traits::critical_context> critical_context_const_ref;
	typedef ddk::intrusive_stack<critical_context_ref> context_intrusive_stack;

public:
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef ddk::intrusive_node<critical_context_ref> context_node_base;

	inline provider_interface* get_provider()
	{
		return static_cast<provider_interface*>(this);
	}
	inline const provider_interface* get_provider() const
	{
		return static_cast<const provider_interface*>(this);
	}
};

}

template<typename Traits>
using IAccessCriticalCallContext = detail::IAccessCriticalCallContextImpl<Traits,std::is_same<typename Traits::critical_context,ddk::critical_section_context>::value>;

}