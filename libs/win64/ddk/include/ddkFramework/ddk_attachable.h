#pragma once

#include "ddk_sync_executor_impl.h"

namespace ddk
{

template<typename>
class attachable;
template<typename>
class async_executor;

template<typename T>
class attachable
{
    friend class async_executor<T>;

public:
    attachable(cancellable_executor_unique_ref<T> i_executorImpl);
    attachable(const attachable&) = delete;
    attachable(attachable&&) = default;
	~attachable() = default;

private:
    cancellable_executor_unique_ref<T> m_executorImpl;
};

template<typename T>
inline attachable<void> attach(thread i_thread);
template<typename T>
inline attachable<void> attach(fiber i_fiber);
template<typename T>
inline attachable<void> attach(cancellable_executor_unique_ref<detail::void_t> i_attachable);

template<>
class attachable<void> : public attachable<detail::void_t>
{
public:
    using attachable<detail::void_t>::attachable;

    attachable(const attachable&) = delete;
    attachable(attachable&&) = default;
	~attachable() = default;
};

attachable<void> attach(thread i_thread);
attachable<void> attach(fiber i_fiber);
attachable<void> attach(thread_sheaf i_threadSheaf);
attachable<void> attach(fiber_sheaf i_fiberSheaf);
attachable<void> attach(cancellable_executor_unique_ref<detail::void_t> i_attachable);

}

#include "ddk_attachable.inl"
