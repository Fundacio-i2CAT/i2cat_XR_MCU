#pragma once

#include "ddk_macros.h"
#include <cstdlib>
#include "ddk_critical_section_context.h"
#include "ddk_critical_section_defs.h"
#include "ddk_iaccess_critical_call_context.h"
#include "ddk_intrusive_node.h"
#include "ddk_lendable.h"
#include "ddk_reference_wrapper.h"
#include <type_traits>

namespace ddk
{
namespace detail
{

struct empty_critical_section_impl_t
{};

template<typename,bool>
class critical_section_impl;

template<typename Traits>
class critical_section_impl<Traits,false>
{
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef IAccessCriticalCallContext<Traits> access_call_context;
	typedef typename access_call_context::critical_call_context critical_call_context;
	typedef typename access_call_context::const_critical_call_context const_critical_call_context;
	template<typename TTraits,bool>
	friend class const_critical_section_impl;

public:
	critical_section_impl(const empty_critical_section_impl_t&)
	: m_callContext(reinterpret_cast<access_call_context*>(0xDEAD))
	, m_id(k_invalidCriticalAccessIdentifier)
	, m_context(context_unacquired)
	{
	}
	explicit critical_section_impl(access_call_context& i_callContext, critical_section_identifier i_id,critical_context i_context)
	: m_callContext(&i_callContext)
	, m_id(i_id)
	, m_context(std::move(i_context))
	{
	}
	critical_section_impl(const critical_section_impl&) = delete;
	critical_section_impl(critical_section_impl&& other)
	: m_callContext(other.m_callContext)
	, m_id(other.m_id)
	, m_context(std::move(other.m_context))
	{
		other.m_id = k_invalidCriticalAccessIdentifier;
		other.m_callContext = nullptr;
	}
	~critical_section_impl()
	{
		//IF YOU RAISED THIS CRASH IS BECAUSE YOU FORGOT TO LEAVE THIS CRITICAL SECTION, I.E. YOU FORGOT TO ABANDON THIS CRITICAL SECTION FOR OTHER PEOPLER USAGE, GO AND PUT IT!!
		if(*m_context)
		{
			MAKE_IT_CRASH
		}
	}
	critical_section_impl& operator=(const critical_section_impl& other) = delete;
	critical_section_impl& operator=(critical_section_impl&& other)
	{
		m_callContext = other.m_callContext;
		m_id = other.m_id;

		other.m_id = k_invalidCriticalAccessIdentifier;
		other.m_callContext = nullptr;

		m_context = std::move(other.m_context);

		return *this;
	}
	inline const_critical_call_context operator->() const
	{
		return m_callContext->create_call_context(ddk::lend(m_context));
	}
	inline critical_call_context operator->()
	{
		return m_callContext->create_call_context(ddk::lend(m_context));
	}
	const_critical_call_context get_call_context() const
	{
		return m_callContext->create_call_context(ddk::lend(m_context));
	}
	provider_interface& get_provider()
	{
		return m_callContext->get_provider();
	}
	bool empty() const
	{
		return static_cast<bool>(*m_context);
	}
	operator bool() const
	{
		return static_cast<bool>(*m_context);
	}
	critical_section_identifier get_id() const
	{
		return m_id;
	}
	critical_context extract_context()
	{
		return std::move(*m_context);
	}

private:
	access_call_context* m_callContext;
	critical_section_identifier m_id;
	mutable ddk::lendable<critical_context> m_context;
};

template<typename Traits>
class critical_section_impl<Traits,true>
{
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef IAccessCriticalCallContext<Traits> access_call_context;
	template<typename TTraits,bool>
	friend class const_critical_section_impl;

public:
	critical_section_impl(const empty_critical_section_impl_t&)
	: m_id(k_invalidCriticalAccessIdentifier)
	, m_context(context_unacquired)
	, m_provider(nullptr)
	{
	}
	explicit critical_section_impl(access_call_context& i_callContext,critical_section_identifier i_id,critical_context i_context)
	: m_id(i_id)
	, m_context(std::move(i_context))
	, m_provider(i_callContext.get_provider())
	{
	}
	critical_section_impl(const critical_section_impl&) = delete;
	critical_section_impl(critical_section_impl&& other)
	: m_id(other.m_id)
	, m_context(std::move(other.m_context))
	, m_provider(nullptr)
	{
		other.m_id = k_invalidCriticalAccessIdentifier;
		std::swap(m_provider,other.m_provider);
	}
	~critical_section_impl()
	{
		//IF YOU RAISED THIS CRASH IS BECAUSE YOU FORGOT TO LEAVE THIS CRITICAL SECTION, I.E. YOU FORGOT TO ABANDON THIS CRITICAL SECTION FOR OTHER PEOPLER USAGE, GO AND PUT IT!!
		if(*m_context)
		{
			MAKE_IT_CRASH
		}
	}
	critical_section_impl& operator=(const critical_section_impl& other) = delete;
	critical_section_impl& operator=(critical_section_impl&& other)
	{
		m_id = other.m_id;
		m_provider = other.m_provider;

		other.m_id = k_invalidCriticalAccessIdentifier;
		other.m_provider = nullptr;

		m_context = std::move(other.m_context);

		return *this;
	}
	inline provider_interface* operator->() const
	{
		return m_provider;
	}
	inline provider_interface* operator->()
	{
		return m_provider;
	}
	bool empty() const
	{
		return static_cast<bool>(*m_context);
	}
	operator bool() const
	{
		return static_cast<bool>(*m_context);
	}
	critical_section_identifier get_id() const
	{
		return m_id;
	}
	critical_context extract_context()
	{
		return std::move(*m_context);
	}

private:
	critical_section_identifier m_id;
	mutable ddk::lendable<critical_context> m_context;
	provider_interface* m_provider;
};

template<typename, bool>
class const_critical_section_impl;

template<typename Traits>
class const_critical_section_impl<Traits,false>
{
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef IAccessCriticalCallContext<Traits> access_call_context;
	typedef typename access_call_context::const_critical_call_context const_critical_call_context;

public:
	const_critical_section_impl(const empty_critical_section_impl_t&)
	: m_callContext(reinterpret_cast<const access_call_context*>(0xDEAD))
	, m_id(critical_section_identifier(0))
	, m_context(context_unacquired)
	{
	}
	explicit const_critical_section_impl(const access_call_context& i_callContext,critical_section_identifier i_id,critical_context i_context)
	: m_callContext(&i_callContext)
	, m_id(i_id)
	, m_context(std::move(i_context))
	{
	}
	const_critical_section_impl(const const_critical_section_impl& other) = delete;
	const_critical_section_impl(const_critical_section_impl&& other)
	: m_callContext(other.m_callContext)
	, m_id(other.m_id)
	, m_context(std::move(other.m_context))
	{
		other.m_id = k_invalidCriticalAccessIdentifier;
		other.m_callContext = nullptr;
	}
	const_critical_section_impl(critical_section_impl<Traits,false>&& other)
	: m_callContext(other.m_callContext)
	, m_id(other.m_id)
	, m_context(std::move(other.m_context))
	{
		other.m_id = k_invalidCriticalAccessIdentifier;
		other.m_callContext = nullptr;
	}
	~const_critical_section_impl()
	{
		//IF YOU RAISED THIS CRASH IS BECAUSE YOU FORGOT TO LEAVE THIS CRITICAL SECTION, I.E. YOU FORGOT TO ABANDON THIS CRITICAL SECTION FOR OTHER PEOPLER USAGE, GO AND PUT IT!!
		if(*m_context)
		{
			MAKE_IT_CRASH
		}
	}
	const_critical_section_impl& operator=(const const_critical_section_impl& other) = delete;
	const_critical_section_impl& operator=(const_critical_section_impl&& other)
	{
		m_callContext = other.m_callContext;
		m_id = other.m_id;
		other.m_id = k_invalidCriticalAccessIdentifier;
		other.m_callContext = nullptr;

		m_context = std::move(other.m_context);

		return *this;
	}
	inline const_critical_call_context operator->() const
	{
		return m_callContext->create_call_context(ddk::lend(m_context));
	}
	inline const provider_interface& get_provider() const
	{
		return m_callContext->get_provider();
	}
	bool empty() const
	{
		return static_cast<bool>(*m_context);
	}
	operator bool() const
	{
		return static_cast<bool>(*m_context);
	}
	critical_section_identifier get_id() const
	{
		return m_id;
	}
	critical_context extract_context()
	{
		return std::move(*m_context);
	}

private:
	const access_call_context* m_callContext;
	critical_section_identifier m_id;
	mutable ddk::lendable<critical_context> m_context;
};

template<typename Traits>
class const_critical_section_impl<Traits,true>
{
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef IAccessCriticalCallContext<Traits> access_call_context;

public:
	const_critical_section_impl(const empty_critical_section_impl_t&)
	: m_id(critical_section_identifier(0))
	, m_context(context_unacquired)
	, m_provider(nullptr)
	{
	}
	explicit const_critical_section_impl(const access_call_context& i_callContext,critical_section_identifier i_id,critical_context i_context)
	: m_id(i_id)
	, m_context(std::move(i_context))
	, m_provider(i_callContext.get_provider())
	{
	}
	const_critical_section_impl(const const_critical_section_impl& other) = delete;
	const_critical_section_impl(const_critical_section_impl&& other)
	: m_id(other.m_id)
	, m_context(std::move(other.m_context))
	, m_provider(nullptr)
	{
		other.m_id = k_invalidCriticalAccessIdentifier;
		std::swap(m_provider,other.m_provider);
	}
	const_critical_section_impl(critical_section_impl<Traits,true>&& other)
	: m_id(other.m_id)
	, m_context(std::move(other.m_context))
	, m_provider(nullptr)
	{
		other.m_id = k_invalidCriticalAccessIdentifier;
		std::swap(m_provider,other.m_provider);
	}
	~const_critical_section_impl()
	{
		//IF YOU RAISED THIS CRASH IS BECAUSE YOU FORGOT TO LEAVE THIS CRITICAL SECTION, I.E. YOU FORGOT TO ABANDON THIS CRITICAL SECTION FOR OTHER PEOPLER USAGE, GO AND PUT IT!!
		if(*m_context)
		{
			MAKE_IT_CRASH
		}
	}
	const_critical_section_impl& operator=(const const_critical_section_impl& other) = delete;
	const_critical_section_impl& operator=(const_critical_section_impl&& other)
	{
		m_id = other.m_id;
		m_provider = other.m_provider;

		other.m_id = k_invalidCriticalAccessIdentifier;
		other.m_callContext = nullptr;

		m_context = std::move(other.m_context);

		return *this;
	}
	inline const provider_interface* operator->() const
	{
		return m_provider;
	}
	bool empty() const
	{
		return static_cast<bool>(*m_context);
	}
	operator bool() const
	{
		return static_cast<bool>(*m_context);
	}
	critical_section_identifier get_id() const
	{
		return m_id;
	}
	critical_context extract_context()
	{
		return std::move(*m_context);
	}

private:
	critical_section_identifier m_id;
	mutable ddk::lendable<critical_context> m_context;
	const provider_interface* m_provider;
};

}

template<typename Traits>
using const_critical_section = detail::const_critical_section_impl<Traits,std::is_same<typename Traits::critical_context,ddk::critical_section_context>::value>;
template<typename Traits>
using critical_section = detail::critical_section_impl<Traits,std::is_same<typename Traits::critical_context,ddk::critical_section_context>::value>;

const detail::empty_critical_section_impl_t empty_critical_section_impl = detail::empty_critical_section_impl_t();

namespace mpl
{

template<typename Traits>
struct is_copy_constructible<critical_section<Traits>>
{
	static const bool value = false;
};
template<typename Traits>
struct is_copy_constructible<const_critical_section<Traits>>
{
	static const bool value = false;
};

}
}
