#pragma once

#include "ddk_mutex.h"
#include "ddk_cond_var.h"
#include "ddk_arena.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_async_executor_interface.h"
#include "ddk_variant.h"
#include "ddk_lend_from_this.h"

namespace ddk
{

template<typename>
class promise;
template<typename>
class executor_promise;

namespace detail
{

struct async_state_base : public lend_from_this<async_state_base>
{
	virtual ~async_state_base() = default;
	virtual void notify() const = 0;
};

template<typename T>
struct private_async_state : async_state_base
{
	template<typename>
	friend struct private_async_state;
	template<typename>
	friend class ddk::promise;
	template<typename>
	friend class ddk::executor_promise;

public:
	struct reference_counter : public distributed_reference_counter
	{
	public:
		reference_counter(private_async_state& i_asyncSharedState);
		unsigned int decrementSharedReference();

	private:
		private_async_state& m_asyncSharedState;
	};

	typedef typename mpl::static_if<std::is_reference<T>::value,typename embedded_type<T>::ref_type,typename mpl::static_if<std::is_copy_constructible<T>::value,embedded_type<T>,embedded_type<T&&>>::type::cref_type>::type sink_type;
	typedef typename embedded_type<T>::ref_type reference;
	typedef typename embedded_type<T>::cref_type const_reference;
	typedef typename embedded_type<T>::rref_type rreference;
	typedef typename async_cancellable_interface::cancel_result cancel_result;

	private_async_state();
	~private_async_state();
	cancel_result cancel();
	void attach(async_cancellable_dist_ptr i_executor);
	void detach();
	template<typename Predicate>
	bool detach_if(Predicate&& i_predicate);
	void set_value(sink_type i_value);
	void set_exception(const async_exception& i_exception);
	void signal() const;
	const_reference get_value() const;
	reference get_value();
	embedded_type<T> extract_value();
	void wait() const;
	void wait_for(unsigned int i_period) const;
	bool pending() const;
	bool ready() const;
	virtual void notify() const override;
	void unsafe_notify() const;
	async_base_dist_ptr get_aync_execution() const;

private:
	mutable mutex m_mutex;
	mutable cond_var m_condVar;
	variant<detail::none_t,async_exception,T> m_arena;
	mutable async_cancellable_dist_ptr m_asyncExecutor;
	reference_counter m_refCounter;
};

template<typename T>
using private_async_state_dist_ref = detail::shared_reference_wrapper_impl<private_async_state<T>,typename private_async_state<T>::reference_counter>;
template<typename T>
using private_async_state_const_dist_ref = detail::shared_reference_wrapper_impl<const private_async_state<T>,typename private_async_state<T>::reference_counter>;
template<typename T>
using private_async_state_dist_ptr = detail::shared_pointer_wrapper_impl<private_async_state<T>,typename private_async_state<T>::reference_counter>;
template<typename T>
using private_async_state_const_dist_ptr = detail::shared_pointer_wrapper_impl<const private_async_state<T>,typename private_async_state<T>::reference_counter>;

template<typename T>
using private_async_state_lent_ref = lent_reference_wrapper<private_async_state<T>>;
template<typename T>
using private_async_state_const_lent_ref = lent_reference_wrapper<const private_async_state<T>>;
template<typename T>
using private_async_state_lent_ptr = lent_pointer_wrapper<private_async_state<T>>;
template<typename T>
using private_async_state_const_lent_ptr = lent_pointer_wrapper<const private_async_state<T>>;

}
}

#include "ddk_async_shared_state.inl"