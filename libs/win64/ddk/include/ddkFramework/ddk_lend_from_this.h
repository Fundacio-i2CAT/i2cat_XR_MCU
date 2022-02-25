#pragma once

#include "ddk_reference_counter.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_macros.h"
#include "ddk_reference_tag.h"
#include <type_traits>

namespace ddk
{

template<typename T, typename TT = T>
class lend_from_this
{
	typedef TT interface_type;

	template<typename TTT, typename TTTT>
	friend inline lent_reference_wrapper<TTTT> lend(lend_from_this<TTT,TTTT>&);
	template<typename TTT,typename TTTT>
	friend inline lent_reference_wrapper<const TTTT> lend(const lend_from_this<TTT,TTTT>&);

public:
	lend_from_this()
	{
		m_counter.addStrongReference();
	}
	lend_from_this(const lend_from_this& other)
	{
		m_counter.addStrongReference();
	}
	~lend_from_this()
	{
		m_counter.removeStrongReference();

		DDK_ASSERT(m_counter.hasLentReferences() == false, "Still lent references alive while destroying lend_from_this");
	}
	inline lent_reference_wrapper<TT> lend()
	{
		return ref_from_this();
	}
	inline lent_reference_wrapper<const TT> lend() const
	{
		return ref_from_this();
	}

protected:
#ifdef DDK_DEBUG
	inline ddk::lent_reference_wrapper<TT> ref_from_this()
	{
		return ddk::lent_reference_wrapper<TT>(static_cast<T*>(this),&m_counter);
	}
	inline ddk::lent_reference_wrapper<const TT> ref_from_this() const
	{
		return ddk::lent_reference_wrapper<const TT>(static_cast<typename std::add_const<T>::type*>(this),&m_counter);
	}
#else
	inline ddk::lent_reference_wrapper<TT> ref_from_this()
	{
		return ddk::lent_reference_wrapper<TT>(static_cast<T*>(this));
	}
	inline ddk::lent_reference_wrapper<const TT> ref_from_this() const
	{
		return ddk::lent_reference_wrapper<const TT>(static_cast<typename std::add_const<T>::type*>(this));
	}
#endif

private:
	mutable ddk::unique_reference_counter m_counter;
};

}
