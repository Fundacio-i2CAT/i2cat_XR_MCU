#pragma once

#include "ddk_shared_reference_wrapper.h"
#include "ddk_unique_pointer_wrapper.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_shared_pointer_wrapper.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_pointer_wrapper.h"
#include "ddk_weak_pointer_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_lend_from_this.h"
#include "ddk_shared_from_this.h"
#include "ddk_distribute_from_this.h"
#include "ddk_lendable.h"
#include "ddk_atomic_shared_pointer_wrapper.h"
#include "ddk_atomic_shared_reference_wrapper.h"
#include "ddk_atomic_weak_pointer_wrapper.h"
#include "ddk_smart_ptr_template_helper.h"
#include "ddk_pointer_defs.h"
#include "ddk_allocator_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

namespace detail
{

template<typename T>
inline unique_pointer_wrapper<T> __make_unique_pointer(T* i_data,const tagged_pointer<unique_reference_counter>& i_refCounter,const tagged_pointer_deleter& i_refDeleter);
template<typename T>
inline unique_pointer_wrapper<T> __make_unique_pointer(T* i_data,tagged_pointer<unique_reference_counter>&& i_refCounter,const tagged_pointer_deleter& i_refDeleter);
template<typename T>
inline unique_reference_wrapper<T> __make_unique_reference(T* i_data,const tagged_pointer<unique_reference_counter>& i_refCounter,const tagged_pointer_deleter& i_refDeleter);
template<typename T>
inline unique_reference_wrapper<T> __make_unique_reference(T* i_data,tagged_pointer<unique_reference_counter>&& i_refCounter,const tagged_pointer_deleter& i_refDeleter);

#ifdef DDK_DEBUG

template<typename T>
inline lent_reference_wrapper<T> __make_lent_reference(T* i_data,const tagged_pointer<lent_reference_counter>& i_refCounter);
template<typename T>
inline lent_pointer_wrapper<T> __make_lent_pointer(T* i_data,const tagged_pointer<lent_reference_counter>& i_refCounter);

#else

template<typename T>
inline lent_reference_wrapper<T> __make_lent_reference(T* i_data);
template<typename T>
inline lent_pointer_wrapper<T> __make_lent_pointer(T* i_data);

#endif

template<typename T,typename ReferenceCounter>
inline shared_reference_wrapper_impl<T,ReferenceCounter> __make_shared_reference(T* i_data,const tagged_pointer<ReferenceCounter>& i_refCounter,const tagged_pointer_deleter& i_refDeleter);
template<typename T>
weak_pointer_wrapper<T> __make_weak_pointer(T* i_data,const tagged_pointer<shared_reference_counter>& i_refCounter,const tagged_pointer_deleter& i_refDeleter);
template<typename T>
inline weak_pointer_wrapper<T> __weak(shared_pointer_wrapper_impl<T,shared_reference_counter>& i_sharedPtr);
template<typename T>
inline weak_pointer_wrapper<const T> __weak(const shared_pointer_wrapper_impl<T,shared_reference_counter>& i_sharedPtr);
template<typename TT,typename T,typename ReferenceCounter>
inline shared_pointer_wrapper_impl<TT,ReferenceCounter> __static_shared_cast(const shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedRef);
template<typename TT,typename T,typename ReferenceCounter>
inline shared_reference_wrapper_impl<TT,ReferenceCounter> __static_shared_cast(const shared_reference_wrapper_impl<T,ReferenceCounter>& i_sharedRef);
template<typename TT,typename T,typename ReferenceCounter>
inline shared_pointer_wrapper_impl<TT,ReferenceCounter> __dynamic_shared_cast(const shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedRef);
template<typename TT,typename T,typename ReferenceCounter>
inline shared_pointer_wrapper_impl<TT,ReferenceCounter> __reinterpret_shared_cast(const shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);
template<typename TT,typename T,typename ReferenceCounter>
inline shared_reference_wrapper_impl<TT,ReferenceCounter> __reinterpret_shared_cast(const shared_reference_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);
template<typename T,typename ReferenceCounter>
inline shared_reference_wrapper_impl<T,ReferenceCounter> __const_shared_cast(const shared_reference_wrapper_impl<const T,ReferenceCounter>& i_sharedRef);
template<typename T,typename ReferenceCounter>
inline shared_pointer_wrapper_impl<T,ReferenceCounter> __const_shared_cast(const shared_pointer_wrapper_impl<const T,ReferenceCounter>& i_sharedPtr);
template<typename T,typename ReferenceCounter>
inline shared_reference_wrapper_impl<T,ReferenceCounter> __promote_to_ref(const shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);
template<typename T,typename ReferenceCounter>
inline lent_pointer_wrapper<T> __lend(shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);
template<typename T,typename ReferenceCounter>
inline lent_pointer_wrapper<T> __lend(const shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);
template<typename T,typename ReferenceCounter>
inline lent_reference_wrapper<T> __lend(shared_reference_wrapper_impl<T,ReferenceCounter>& i_sharedRef);
template<typename T,typename ReferenceCounter>
inline lent_reference_wrapper<T> __lend(const shared_reference_wrapper_impl<T,ReferenceCounter>& i_sharedRef);

}

template<typename T>
constexpr size_t size_of_unique_allocation();

template<typename T, typename ... Args>
inline unique_reference_wrapper<T> make_unique_reference(Args&& ... i_args);

template<typename T>
inline unique_reference_wrapper<T> as_unique_reference(T* i_ptr,const tagged_pointer_deleter& i_refDeleter);

template<typename T>
inline unique_reference_wrapper<T> as_unique_reference(T* i_ptr);

template<typename T>
inline unique_reference_wrapper<T> as_unique_reference(T* i_ptr, const tagged_pointer<unique_reference_counter>& i_refCounter, const tagged_pointer_deleter& i_refDeleter);

template<typename T>
constexpr size_t size_of_shared_allocation();

template<typename T, typename ... Args>
inline shared_reference_wrapper<T> make_shared_reference(Args&& ... i_args);

template<typename T>
inline shared_reference_wrapper<T> as_shared_reference(T* i_ptr);
template<typename T>
inline shared_reference_wrapper<T> as_shared_reference(T* i_ptr, const tagged_pointer_deleter& i_refDeleter);

template<typename T>
constexpr size_t size_of_distributed_allocation();

template<typename T,typename ... Args>
inline distributed_reference_wrapper<T> make_distributed_reference(Args&& ... i_args);

template<typename T>
inline distributed_reference_wrapper<T> as_distributed_reference(T* i_ptr);
template<typename T>
inline distributed_reference_wrapper<T> as_distributed_reference(T* i_ptr,const tagged_pointer_deleter& i_refDeleter);

template<typename T, typename ReferenceCounter>
inline detail::shared_reference_wrapper_impl<T,ReferenceCounter> as_shared_reference(T* i_ptr, const tagged_pointer<ReferenceCounter>& i_refCounter, const tagged_pointer_deleter& i_refDeleter);

template<typename T,typename ... Args>
inline atomic_shared_reference_wrapper<T> make_atomic_shared_reference(Args&& ... i_args);
template<typename T,typename ... Args>
inline atomic_distributed_reference_wrapper<T> make_atomic_distributed_reference(Args&& ... i_args);

template<typename T>
inline atomic_shared_reference_wrapper<T> as_atomic_shared_reference(T* i_ptr);
template<typename T>
inline atomic_shared_reference_wrapper<T> as_atomic_shared_reference(T* i_ptr,const tagged_pointer_deleter& i_refDeleter);
template<typename T>
inline atomic_distributed_reference_wrapper<T> as_atomic_shared_reference(T* i_ptr,const tagged_pointer<shared_reference_counter>& i_refCounter,const tagged_pointer_deleter& i_refDeleter);

template<typename T>
inline atomic_distributed_reference_wrapper<T> as_atomic_distributed_reference(T* i_ptr);
template<typename T>
inline atomic_distributed_reference_wrapper<T> as_atomic_distributed_reference(T* i_ptr,const tagged_pointer_deleter& i_refDeleter);
template<typename T>
inline atomic_distributed_reference_wrapper<T> as_atomic_shared_reference(T* i_ptr,const tagged_pointer<distributed_reference_counter>& i_refCounter,const tagged_pointer_deleter& i_refDeleter);

template<typename TT, typename T, typename ReferenceCounter>
inline auto dynamic_shared_cast(const detail::shared_reference_wrapper_impl<T,ReferenceCounter>& i_sharedRef);

template<typename TT, typename T>
inline unique_pointer_wrapper<TT> dynamic_unique_cast(unique_reference_wrapper<T>&& i_uniqueRef);

#ifdef DDK_DEBUG

template<typename TT, typename T>
inline lent_pointer_wrapper<TT> dynamic_lent_cast(const lent_reference_wrapper<T>& i_lentRef);

#endif

template<typename TT, typename T, typename ReferenceCounter>
inline auto dynamic_shared_cast(const detail::shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedRef);

template<typename TT, typename T>
inline unique_pointer_wrapper<TT> dynamic_unique_cast(unique_pointer_wrapper<T>&& i_uniqueRef);

template<typename TT, typename T>
inline lent_pointer_wrapper<TT> dynamic_lent_cast(const lent_pointer_wrapper<T>& i_lentRef);

template<typename TT,typename T,typename ReferenceCounter>
inline auto static_shared_cast(const detail::shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedRef);

template<typename TT, typename T, typename ReferenceCounter>
inline auto static_shared_cast(const detail::shared_reference_wrapper_impl<T,ReferenceCounter>& i_sharedRef);

template<typename TT, typename T>
inline unique_reference_wrapper<TT> static_unique_cast(unique_reference_wrapper<T> i_uniqueRef);

template<typename TT, typename T>
inline unique_pointer_wrapper<TT> static_unique_cast(unique_pointer_wrapper<T> i_uniqueRef);

template<typename TT, typename T>
inline lent_pointer_wrapper<TT> static_lent_cast(const lent_pointer_wrapper<T>& i_lentRef);

#ifdef DDK_DEBUG

template<typename TT, typename T>
inline lent_reference_wrapper<TT> static_lent_cast(const lent_reference_wrapper<T>& i_lentRef);

#endif

template<typename TT, typename T, typename ReferenceCounter>
inline detail::shared_pointer_wrapper_impl<TT,ReferenceCounter> reinterpret_shared_cast(const detail::shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);

template<typename TT,typename T,typename ReferenceCounter>
inline detail::shared_reference_wrapper_impl<TT,ReferenceCounter> reinterpret_shared_cast(const detail::shared_reference_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);

template<typename TT, typename T>
inline unique_pointer_wrapper<TT> reinterpret_unique_cast(unique_pointer_wrapper<T> i_uniquePtr);

template<typename TT, typename T>
inline unique_reference_wrapper<TT> reinterpret_unique_cast(unique_reference_wrapper<T> i_uniqueRef);

template<typename TT, typename T>
inline lent_pointer_wrapper<TT> reinterpret_lent_cast(const lent_pointer_wrapper<T>& i_lentRef);

#ifdef DDK_DEBUG

template<typename TT, typename T>
inline lent_reference_wrapper<TT> reinterpret_lent_cast(const lent_reference_wrapper<T>& i_lentRef);

#endif

template<typename T>
inline unique_pointer_wrapper<T> const_unique_cast(unique_pointer_wrapper<const T> i_uniquePtr);
template<typename T>
inline unique_reference_wrapper<T> const_unique_cast(unique_reference_wrapper<const T> i_uniqueRef);

template<typename T, typename ReferenceCounter>
inline detail::shared_reference_wrapper_impl<T,ReferenceCounter> const_shared_cast(const detail::shared_reference_wrapper_impl<const T,ReferenceCounter>& i_sharedRef);
template<typename T, typename ReferenceCounter>
inline detail::shared_pointer_wrapper_impl<T,ReferenceCounter> const_shared_cast(const detail::shared_pointer_wrapper_impl<const T,ReferenceCounter>& i_sharedPtr);

template<typename T>
inline lent_pointer_wrapper<T> const_lent_cast(const lent_pointer_wrapper<const T>& i_lentPtr);

#ifdef DDK_DEBUG

template<typename T>
inline lent_reference_wrapper<T> const_lent_cast(const lent_reference_wrapper<const T>& i_lentRef);

#endif

template<typename T>
inline lent_pointer_wrapper<T> lend(unique_pointer_wrapper<T>& i_uniquePtr);
template<typename T>
inline lent_pointer_wrapper<T> lend(const unique_pointer_wrapper<T>& i_uniquePtr);

template<typename T>
inline lent_reference_wrapper<T> lend(unique_reference_wrapper<T>& i_uniqueRef);
template<typename T>
inline lent_reference_wrapper<T> lend(const unique_reference_wrapper<T>& i_uniqueRef);

template<typename T, typename ReferenceCounter>
inline lent_pointer_wrapper<T> lend(detail::shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);
template<typename T, typename ReferenceCounter>
inline lent_pointer_wrapper<T> lend(const detail::shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);

template<typename T, typename ReferenceCounter>
inline lent_reference_wrapper<T> lend(detail::shared_reference_wrapper_impl<T,ReferenceCounter>& i_sharedRef);
template<typename T, typename ReferenceCounter>
inline lent_reference_wrapper<T> lend(const detail::shared_reference_wrapper_impl<T,ReferenceCounter>& i_sharedRef);

template<typename T,typename ReferenceCounter>
inline lent_pointer_wrapper<T> lend(detail::atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);
template<typename T,typename ReferenceCounter>
inline lent_pointer_wrapper<T> lend(const detail::atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);
template<typename T,typename ReferenceCounter>
inline lent_reference_wrapper<T> lend(detail::atomic_shared_reference_wrapper_impl<T,ReferenceCounter>& i_sharedRef);
template<typename T,typename ReferenceCounter>
inline lent_reference_wrapper<T> lend(const detail::atomic_shared_reference_wrapper_impl<T,ReferenceCounter>& i_sharedRef);

template<typename T, typename TT>
inline lent_reference_wrapper<TT> lend(lend_from_this<T,TT>& i_lendable);
template<typename T, typename TT>
inline lent_reference_wrapper<const TT> lend(const lend_from_this<T,TT>& i_lendable);

template<typename T>
inline lent_reference_wrapper<T> lend(lendable<T>& i_lendable);
template<typename T>
inline lent_reference_wrapper<const T> lend(const lendable<T>& i_lendable);

template<typename T,typename TT>
inline lent_reference_wrapper<TT> lend(share_from_this<T,TT>& i_sharedFromThis);
template<typename T,typename TT>
inline lent_reference_wrapper<const TT> lend(const share_from_this<T,TT>& i_sharedFromThis);

template<typename T,typename TT>
inline weak_pointer_wrapper<TT> weak(share_from_this<T,TT>& i_sharedFromThis);
template<typename T,typename TT>
inline weak_pointer_wrapper<const TT> weak(const share_from_this<TT>& i_sharedFromThis);
template<typename T>
inline weak_pointer_wrapper<T> weak(shared_pointer_wrapper<T>& i_sharedPtr);
template<typename T>
inline weak_pointer_wrapper<const T> weak(const shared_pointer_wrapper<T>& i_sharedPtr);

template<typename T>
inline atomic_weak_pointer_wrapper<T> weak(atomic_shared_pointer_wrapper<T>& i_sharedPtr);
template<typename T>
inline atomic_weak_pointer_wrapper<const T> weak(const atomic_shared_pointer_wrapper<T>& i_sharedPtr);

template<typename T, typename TT>
inline shared_reference_wrapper<const TT> share(const share_from_this<T,TT>& i_shareFromThis);
template<typename T, typename TT>
inline shared_reference_wrapper<TT> share(share_from_this<T,TT>& i_shareFromThis);

template<typename T>
inline shared_pointer_wrapper<const T> share(const weak_pointer_wrapper<T>& i_weakPtr);
template<typename T>
inline shared_pointer_wrapper<T> share(weak_pointer_wrapper<T>& i_weakPtr);

template<typename T,typename TT>
inline distributed_reference_wrapper<TT> distribute(distribute_from_this<T,TT>& i_distFromThis);
template<typename T,typename TT>
inline distributed_reference_wrapper<const TT> distribute(const distribute_from_this<T,TT>& i_distFromThis);

template<typename T>
inline atomic_shared_pointer_wrapper<const T> share(const atomic_weak_pointer_wrapper<T>& i_weakPtr);
template<typename T>
inline atomic_shared_pointer_wrapper<T> share(atomic_weak_pointer_wrapper<T>& i_weakPtr);

template<typename T>
inline unique_reference_wrapper<T> promote_to_ref(unique_pointer_wrapper<T> i_uniquePtr);
template<typename T, typename ReferenceCounter>
inline detail::shared_reference_wrapper_impl<T,ReferenceCounter> promote_to_ref(const detail::shared_pointer_wrapper_impl<T,ReferenceCounter>& i_sharedPtr);
template<typename T>
inline lent_reference_wrapper<T> promote_to_ref(const lent_pointer_wrapper<T>& i_lentPtr);

template<typename T>
inline bool operator==(const ddk::unique_pointer_wrapper<T>& i_lhs, const ddk::unique_pointer_wrapper<T>& i_rhs);
template<typename T>
inline bool operator==(const ddk::unique_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs);
template<typename T>
inline bool operator==(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::unique_pointer_wrapper<T>& i_rhs);
#ifdef DDK_DEBUG
template<typename T>
inline bool operator==(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs);
#endif
template<typename T>
inline bool operator!=(const ddk::unique_pointer_wrapper<T>& i_lhs, const ddk::unique_pointer_wrapper<T>& i_rhs);
template<typename T>
inline bool operator!=(const ddk::unique_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs);
template<typename T>
inline bool operator!=(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::unique_pointer_wrapper<T>& i_rhs);
#ifdef DDK_DEBUG
template<typename T>
inline bool operator!=(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs);
#endif
template<typename T>
inline bool operator==(const ddk::shared_pointer_wrapper<T>& i_lhs, const ddk::shared_pointer_wrapper<T>& i_rhs);
template<typename T>
inline bool operator==(const ddk::shared_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs);
template<typename T>
inline bool operator==(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::shared_pointer_wrapper<T>& i_rhs);

template<typename T>
inline bool operator!=(const ddk::shared_pointer_wrapper<T>& i_lhs, const ddk::shared_pointer_wrapper<T>& i_rhs);
template<typename T>
inline bool operator!=(const ddk::shared_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs);
template<typename T>
inline bool operator!=(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::shared_pointer_wrapper<T>& i_rhs);

template<typename T>
struct smart_pointer_compare
{
public:
	smart_pointer_compare() = default;
	inline bool operator()(const T& i_lhs, const T& i_rhs) const
	{
		return get_raw_ptr(i_lhs) < get_raw_ptr(i_rhs);
	}
};

}

#include "ddk_reference_wrapper.inl"
