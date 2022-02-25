
#include <thread>

namespace ddk
{
namespace detail
{

template<typename T>
struct lock_unlocker
{
public:
    inline bool unlock()
    {
        m_lock->unlock();
        return true;
    }

private:
    T* m_lock = nullptr;
};

template<typename T>
lock_guard_impl<mpl::sequence<>,T>::lock_guard_impl(T& i_lockableObject)
: m_lockableObject(i_lockableObject)
{
    m_lockableObject.lock();
}
template<typename T>
lock_guard_impl<mpl::sequence<>,T>::~lock_guard_impl()
{
    m_lockableObject.unlock();
}

template<size_t ... Indexs,typename T, typename ... TT>
lock_guard_impl<mpl::sequence<Indexs...>,T,TT...>::lock_guard_impl(T& i_lockableObject, TT& ... i_lockableObjects)
: m_primaryLock(i_lockableObject)
{
    static const size_t s_numBytes = (s_numSecondaryLocks / 8) + 1;
    static const unsigned int s_allIn = (1 << s_numSecondaryLocks) - 1;

    const void* _[] = { m_secondaryLocks[Indexs] = &i_lockableObjects ... };

acquire_locks:

    m_primaryLock.lock();

    const unsigned int tryLockRes = ((static_cast<unsigned int>(reinterpret_cast<TT*>(m_secondaryLocks[Indexs])->try_lock()) << Indexs) | ...);

    if(memcmp(&tryLockRes,&s_allIn,s_numBytes) != 0)
    {
        ( (static_cast<bool>(tryLockRes & (1 << Indexs)) && reinterpret_cast<detail::lock_unlocker<TT>*>(m_secondaryLocks[Indexs])->unlock()) | ... );

        m_primaryLock.unlock();

        std::this_thread::yield();

        goto acquire_locks;
    }
}
template<size_t ... Indexs,typename T, typename ... TT>
lock_guard_impl<mpl::sequence<Indexs...>,T,TT...>::~lock_guard_impl()
{
    ( reinterpret_cast<detail::lock_unlocker<TT>*>(m_secondaryLocks[Indexs])->unlock() | ... );

    m_primaryLock.unlock();
}

}
}
