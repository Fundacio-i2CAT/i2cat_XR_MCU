
#include "ddk_macros.h"

namespace ddk
{

template<size_t ArenaSize, size_t ArenaAlignment>
arena<ArenaSize,ArenaAlignment>::arena()
: m_empty(true)
{
	memset(&m_storage,0,ArenaSize);
}
template<size_t ArenaSize, size_t ArenaAlignment>
arena<ArenaSize,ArenaAlignment>::~arena()
{
	DDK_ASSERT(m_empty, "Destroying arena without getting rid of its contents");
}
template<size_t ArenaSize, size_t ArenaAlignment>
template<typename T, typename ... Args>
void arena<ArenaSize,ArenaAlignment>::construct(Args&& ... i_args)
{
	embedded_type<T>::construct(&m_storage,std::forward<Args>(i_args) ...);

	m_empty = false;
}
template<size_t ArenaSize, size_t ArenaAlignment>
template<typename T, typename ... Args>
void arena<ArenaSize,ArenaAlignment>::assign(Args&& ... i_args)
{
	embedded_type<T>::assign(&m_storage,std::forward<Args>(i_args) ...);

	m_empty = false;
}
template<size_t ArenaSize, size_t ArenaAlignment>
template<typename T, typename ... Args>
void arena<ArenaSize,ArenaAlignment>::set_value(Args&& ... i_args)
{
	if(m_empty)
	{
		embedded_type<T>::construct(&m_storage,std::forward<Args>(i_args) ...);
	}
	else
	{
		embedded_type<T>::assign(&m_storage,std::forward<Args>(i_args) ...);
	}

	m_empty = false;
}
template<size_t ArenaSize, size_t ArenaAlignment>
template<typename T>
void arena<ArenaSize,ArenaAlignment>::destroy()
{
	if(m_empty == false)
	{
		embedded_type<T>::destroy(&m_storage);

		m_empty = true;
	}
}
template<size_t ArenaSize, size_t ArenaAlignment>
template<typename T>
typename embedded_type<T>::cref_type arena<ArenaSize,ArenaAlignment>::get() const
{
	DDK_ASSERT(m_empty == false, "Accessing empty arena");

	const embedded_type<T>* nestedData = reinterpret_cast<const embedded_type<T>*>(&m_storage);

	return nestedData->get();
}
template<size_t ArenaSize, size_t ArenaAlignment>
template<typename T>
typename embedded_type<T>::ref_type arena<ArenaSize,ArenaAlignment>::get()
{
	DDK_ASSERT(m_empty == false, "Accessing empty arena");

	embedded_type<T>* nestedData = reinterpret_cast<embedded_type<T>*>(&m_storage);

	return nestedData->get();
}
template<size_t ArenaSize, size_t ArenaAlignment>
template<typename T>
embedded_type<T> arena<ArenaSize,ArenaAlignment>::extract()
{
	DDK_ASSERT(m_empty == false, "Accessing empty arena");

	embedded_type<T>* nestedData = reinterpret_cast<embedded_type<T>*>(&m_storage);

	embedded_type<T> res(nestedData->extract());

	embedded_type<T>::destroy(&m_storage);

	m_empty = true;

	return std::move(res);
}
template<size_t ArenaSize, size_t ArenaAlignment>
template<typename T>
typename embedded_type<T>::cpointer_type arena<ArenaSize,ArenaAlignment>::get_ptr() const
{
	return reinterpret_cast<const T*>(&m_storage);
}
template<size_t ArenaSize, size_t ArenaAlignment>
template<typename T>
typename embedded_type<T>::pointer_type arena<ArenaSize,ArenaAlignment>::get_ptr()
{
	return reinterpret_cast<T*>(&m_storage);
}
template<size_t ArenaSize, size_t ArenaAlignment>
char* arena<ArenaSize,ArenaAlignment>::get_arena()
{
    return reinterpret_cast<char*>(&m_storage);
}
template<size_t ArenaSize, size_t ArenaAlignment>
const char* arena<ArenaSize,ArenaAlignment>::get_arena() const
{
    return reinterpret_cast<const char*>(&m_storage);
}
template<size_t ArenaSize, size_t ArenaAlignment>
bool arena<ArenaSize,ArenaAlignment>::empty() const
{
	return m_empty;
}
template<size_t ArenaSize, size_t ArenaAlignment>
template<typename T, typename Arg>
bool arena<ArenaSize,ArenaAlignment>::compare(Arg&& i_arg) const
{
	return *reinterpret_cast<const T*>(&m_storage) == i_arg;
}

}
