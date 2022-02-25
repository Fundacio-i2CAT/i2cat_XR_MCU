
namespace ddk
{

template<typename T>
atomic8<T>::atomic8(const T& i_value)
: m_value(i_value)
{
}
template<typename T>
atomic8<T>::atomic8(T&& i_value)
: m_value(std::move(i_value))
{
}
template<typename T>
atomic8<T>::atomic8(const atomic8& other)
: m_value(other.m_value)
{
}
template<typename T>
atomic8<T>::atomic8(atomic8&& other)
: m_value(std::move(other.m_value))
{
}
template<typename T>
atomic8<T>& atomic8<T>::operator=(const atomic8& other)
{
	m_value = other.m_value;
}
template<typename T>
atomic8<T>& atomic8<T>::operator=(atomic8&& other)
{
	m_value = std::move(other.m_value);
}
template<typename T>
const T& atomic8<T>::get() const
{
	return m_value;
}
template<typename T>
T& atomic8<T>::get()
{
	return m_value;
}
template<typename T>
void atomic8<T>::set(const T& i_value)
{
	m_value = i_value;
}
template<typename T>
void atomic8<T>::set(T&& i_value)
{
	m_value = std::move(i_value);
}
template<typename T>
T* atomic8<T>::_get_typed_arena()
{
	return &m_value;
}
template<typename T>
char* atomic8<T>::_get_arena()
{
	return reinterpret_cast<char*>(&m_value);
}

template<typename T>
atomic32<T>::atomic32(const T& i_value)
: m_value(i_value)
{
}
template<typename T>
atomic32<T>::atomic32(T&& i_value)
: m_value(std::move(i_value))
{
}
template<typename T>
atomic32<T>::atomic32(const atomic32& other)
: m_value(other.m_value)
{
}
template<typename T>
atomic32<T>::atomic32(atomic32&& other)
: m_value(std::move(other.m_value))
{
}
template<typename T>
atomic32<T>& atomic32<T>::operator=(const atomic32& other)
{
	m_value = other.m_value;

	return *this;
}
template<typename T>
atomic32<T>& atomic32<T>::operator=(atomic32&& other)
{
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
const T& atomic32<T>::get() const
{
	return m_value;
}
template<typename T>
T& atomic32<T>::get()
{
	return m_value;
}
template<typename T>
void atomic32<T>::set(const T& i_value)
{
	m_value = i_value;
}
template<typename T>
void atomic32<T>::set(T&& i_value)
{
	m_value = std::move(i_value);
}
template<typename T>
T* atomic32<T>::_get_typed_arena()
{
	return &m_value;
}
template<typename T>
uint32_t* atomic32<T>::_get_arena()
{
	return reinterpret_cast<uint32_t*>(&m_value);
}

template<typename T>
atomic64<T>::atomic64(const T& i_value)
: m_value(i_value)
{
}
template<typename T>
atomic64<T>::atomic64(T&& i_value)
: m_value(std::move(i_value))
{
}
template<typename T>
atomic64<T>::atomic64(const atomic64& other)
: m_value(other.m_value)
{
}
template<typename T>
atomic64<T>::atomic64(atomic64&& other)
: m_value(std::move(other.m_value))
{
}
template<typename T>
atomic64<T>& atomic64<T>::operator=(const atomic64& other)
{
	m_value = other.m_value;

	return *this;
}
template<typename T>
atomic64<T>& atomic64<T>::operator=(atomic64&& other)
{
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
const T& atomic64<T>::get() const
{
	return m_value;
}
template<typename T>
T& atomic64<T>::get()
{
	return m_value;
}
template<typename T>
void atomic64<T>::set(const T& i_value)
{
	m_value = i_value;
}
template<typename T>
void atomic64<T>::set(T&& i_value)
{
	m_value = std::move(i_value);
}
template<typename T>
T* atomic64<T>::_get_typed_arena()
{
	return &m_value;
}
template<typename T>
int64_t* atomic64<T>::_get_arena()
{
	return reinterpret_cast<int64_t*>(&m_value);
}

template<typename T>
atomic<T*>::atomic(T* i_value)
{
	memcpy(&m_arena,&i_value,sizeof(T*));
}
template<typename T>
const T* atomic<T*>::get() const
{
	return m_arena;
}
template<typename T>
T* atomic<T*>::get()
{
	return m_arena;
}
template<typename T>
void atomic<T*>::set(T* i_value)
{
	memcpy(&m_arena,&i_value,sizeof(T*));
}
template<typename T>
atomic_uintptr_t& atomic<T*>::as_number()
{
	return reinterpret_cast<atomic_uintptr_t&>(*this);
}
template<typename T>
const atomic_uintptr_t& atomic<T*>::as_number() const
{
	return reinterpret_cast<const atomic_uintptr_t&>(*this);
}
template<typename T>
void* volatile* atomic<T*>::_get_arena()
{
	return reinterpret_cast<void* volatile*>(&m_arena);
}

template<typename T>
bool atomic_compare_exchange(atomic8<T>& i_atomic,const T& i_expectedValue,const T& i_desiredValue)
{
	#if defined(WIN32)
	return _InterlockedCompareExchange8(i_atomic._get_arena(),i_desiredValue,i_expectedValue) == static_cast<char>(i_expectedValue);
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_bool_compare_and_swap(i_atomic._get_typed_arena(),i_expectedValue,i_desiredValue);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
bool atomic_compare_exchange(atomic32<T>& i_atomic,const T& i_expectedValue,const T& i_desiredValue)
{
	#if defined(WIN32)
	return InterlockedCompareExchange(reinterpret_cast<LONG*>(i_atomic._get_arena()),i_desiredValue,i_expectedValue) == i_expectedValue;
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_bool_compare_and_swap(i_atomic._get_typed_arena(),i_expectedValue,i_desiredValue);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
bool atomic_compare_exchange(atomic64<T>& i_atomic,const T& i_expectedValue,const T& i_desiredValue)
{
	#ifdef _WIN32
	return InterlockedCompareExchange64(i_atomic._get_arena(),i_desiredValue,i_expectedValue) == i_expectedValue;
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_bool_compare_and_swap(i_atomic._get_typed_arena(),i_expectedValue,i_desiredValue);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
bool atomic_compare_exchange(atomic<T*>& i_atomic,T* i_expectedValue,T* i_desiredValue)
{
	#ifdef _WIN32
	return InterlockedCompareExchangePointer(i_atomic._get_arena(),i_desiredValue,i_expectedValue) == i_expectedValue;
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_bool_compare_and_swap(i_atomic.m_arena,i_expectedValue,i_desiredValue);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_compare_exchange_val(atomic8<T>& i_atomic,const T& i_expectedValue,const T& i_desiredValue)
{
	#ifdef _WIN32
	return T(_InterlockedCompareExchange8(i_atomic._get_arena(),i_desiredValue,i_expectedValue));
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_val_compare_and_swap(i_atomic._get_typed_arena(),i_expectedValue,i_desiredValue);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_compare_exchange_val(atomic32<T>& i_atomic,const T& i_expectedValue,const T& i_desiredValue)
{
	#ifdef _WIN32
	return T(InterlockedCompareExchange(reinterpret_cast<LONG*>(i_atomic._get_arena()),i_desiredValue,i_expectedValue));
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_val_compare_and_swap(i_atomic._get_typed_arena(),i_expectedValue,i_desiredValue);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_compare_exchange_val(atomic64<T>& i_atomic,const T& i_expectedValue,const T& i_desiredValue)
{
	#ifdef _WIN32
	return T(InterlockedCompareExchange64(i_atomic._get_arena(),i_desiredValue,i_expectedValue));
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_val_compare_and_swap(i_atomic._get_typed_arena(),i_expectedValue,i_desiredValue);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T* atomic_compare_exchange_val(atomic<T*>& i_atomic,T* i_expectedValue,T* i_desiredValue)
{
	#ifdef _WIN32
	return T(InterlockedCompareExchangePointer(i_atomic._get_arena(),i_desiredValue,i_expectedValue));
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_val_compare_and_swap(i_atomic.m_arena,i_expectedValue,i_desiredValue);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_pre_increment(atomic8<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedIncrement16(reinterpret_cast<SHORT*>(i_atomic._get_arena())) - 1);
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_fetch_and_add(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_pre_increment(atomic32<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedIncrement(reinterpret_cast<LONG*>(i_atomic._get_arena())) - 1);
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_fetch_and_add(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_pre_increment(atomic64<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedIncrement64(i_atomic._get_arena()) - 1);
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_fetch_and_add(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_post_increment(atomic8<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedIncrement16(reinterpret_cast<SHORT*>(i_atomic._get_arena())));
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_add_and_fetch(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_post_increment(atomic32<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedIncrement(i_atomic._get_arena()));
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_add_and_fetch(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_post_increment(atomic64<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedIncrement64(i_atomic._get_arena()));
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_add_and_fetch(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_pre_decrement(atomic8<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedDecrement16(reinterpret_cast<SHORT*>(i_atomic._get_arena())) + 1);
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_fetch_and_sub(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_pre_decrement(atomic32<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedDecrement(reinterpret_cast<LONG*>(i_atomic._get_arena())) + 1);
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_fetch_and_sub(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_pre_decrement(atomic64<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedDecrement64(i_atomic._get_arena()) + 1);
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_fetch_and_sub(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_post_decrement(atomic8<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedDecrement16(reinterpret_cast<SHORT*>(i_atomic._get_arena())));
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_sub_and_fetch(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_post_decrement(atomic32<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedDecrement(reinterpret_cast<LONG*>(i_atomic._get_arena())));
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_sub_and_fetch(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}
template<typename T>
inline T atomic_post_decrement(atomic64<T>& i_atomic)
{
	#ifdef _WIN32
	return T(InterlockedDecrement64(i_atomic._get_arena()));
	#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_sub_and_fetch(i_atomic._get_typed_arena(),1);
	#else
	#error "Unsupported platform"
	#endif
}

}
