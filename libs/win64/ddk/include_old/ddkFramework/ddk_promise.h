#pragma once

#include "ddk_future.h"
#include "ddk_async_shared_state.h"

namespace ddk
{

template<typename T>
class promise
{
	template<typename>
	friend class executor_promise;

public:
	typedef typename detail::private_async_state<T>::sink_type sink_type;
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;
	typedef typename detail::private_async_state<T>::rreference rreference;

	promise();
	promise(const promise<T>&);
	promise& operator=(const promise<T>& other);
	void set_value(sink_type i_value);
	void set_exception(const async_exception& i_exception);
	future<T> get_future() const;

protected:
	void signal() const;
	void wait() const;
	void wait_for(unsigned int i_period) const;
	bool ready() const;

	detail::private_async_state_dist_ptr<T> m_sharedState;
};

template<>
class promise<void> : public promise<detail::void_t>
{
};

}
