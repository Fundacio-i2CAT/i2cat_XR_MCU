
namespace ddk
{

#ifdef DDK_DEBUG

template<typename T>
lent_pointer_wrapper<T>::lent_pointer_wrapper(T* i_data,const tagged_pointer<lent_reference_counter>& i_refCounter)
: m_data(i_data)
, m_refCounter(i_refCounter)
{
	if(m_refCounter)
	{
		m_refCounter->incrementLentReference();
	}

	REGISTER_STACK_TRACE(THIS_OBJECT);
}
template<typename T>
lent_pointer_wrapper<T>::lent_pointer_wrapper()
: m_data(nullptr)
, m_refCounter(nullptr)
{
}
template<typename T>
lent_pointer_wrapper<T>::lent_pointer_wrapper(const std::nullptr_t&)
: m_data(nullptr)
, m_refCounter(nullptr)
{
}
template<typename T>
lent_pointer_wrapper<T>::lent_pointer_wrapper(const lent_pointer_wrapper& other)
: m_data(nullptr)
, m_refCounter(nullptr)
{
	m_data = other.m_data;

	if(m_refCounter = other.m_refCounter)
	{
		m_refCounter->incrementLentReference();
	}

	REGISTER_STACK_TRACE(THIS_OBJECT);
}
template<typename T>
lent_pointer_wrapper<T>::lent_pointer_wrapper(lent_pointer_wrapper&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
{
	std::swap(m_data,other.m_data);
	std::swap(m_refCounter,other.m_refCounter);

	MOVE_STACCK_TRACE(other,THIS_OBJECT)
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_pointer_wrapper<T>::lent_pointer_wrapper(const lent_pointer_wrapper<TT>& other)
: m_data(nullptr)
, m_refCounter(nullptr)
{
	m_data = other.m_data;

	if(m_refCounter = other.m_refCounter)
	{
		m_refCounter->incrementLentReference();
	}

	REGISTER_STACK_TRACE(THIS_OBJECT);
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_pointer_wrapper<T>::lent_pointer_wrapper(lent_pointer_wrapper<TT>&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
{
	if((m_data = other.m_data))
	{
		std::swap(m_refCounter,other.m_refCounter);
	}

	other.m_data = nullptr;

	MOVE_STACCK_TRACE(other,THIS_OBJECT)
}
template<typename T>
lent_pointer_wrapper<T>::~lent_pointer_wrapper()
{
	UNREGISTER_STACK_TRACE(THIS_OBJECT)

	if(m_refCounter)
	{
		m_refCounter->decrementLentReference();
	}

	m_data = nullptr;
}
template<typename T>
lent_pointer_wrapper<T>& lent_pointer_wrapper<T>::operator=(const std::nullptr_t&)
{
	UNREGISTER_STACK_TRACE(THIS_OBJECT)

	if(m_refCounter)
	{
		m_refCounter->decrementLentReference();
	}

	m_data = nullptr;
	m_refCounter = nullptr;

	return *this;
}
template<typename T>
lent_pointer_wrapper<T>& lent_pointer_wrapper<T>::operator=(const lent_pointer_wrapper& other)
{
	if(m_data != other.m_data)
	{
		UNREGISTER_STACK_TRACE(THIS_OBJECT)

		if(m_refCounter)
		{
			m_refCounter->decrementLentReference();
		}

		m_data = other.m_data;

		if(m_refCounter = other.m_refCounter)
		{
			m_refCounter->incrementLentReference();
		}

		COPY_STACK_TRACE(other,THIS_OBJECT)
	}

	return *this;
}
template<typename T>
lent_pointer_wrapper<T>& lent_pointer_wrapper<T>::operator=(lent_pointer_wrapper&& other)
{
	if(m_data != other.m_data)
	{
		UNREGISTER_STACK_TRACE(THIS_OBJECT)

		if(m_refCounter)
		{
			m_refCounter->decrementLentReference();
		}

		m_data = other.m_data;
		other.m_data = nullptr;
		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;

		MOVE_STACCK_TRACE(other,THIS_OBJECT)
	}

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_pointer_wrapper<T>& lent_pointer_wrapper<T>::operator=(const lent_pointer_wrapper<TT>& other)
{
	if(m_data != static_cast<const T*>(other.m_data))
	{
		UNREGISTER_STACK_TRACE(THIS_OBJECT)

		if(m_refCounter)
		{
			m_refCounter->decrementLentReference();
		}

		m_data = other.m_data;

		if(m_refCounter = other.m_refCounter)
		{
			m_refCounter->incrementLentReference();
		}

		COPY_STACK_TRACE(other,THIS_OBJECT)
	}

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_pointer_wrapper<T>& lent_pointer_wrapper<T>::operator=(lent_pointer_wrapper<TT>&& other)
{
	if(m_data != static_cast<const T*>(other.m_data))
	{
		UNREGISTER_STACK_TRACE(THIS_OBJECT)

		if(m_refCounter)
		{
			m_refCounter->decrementLentReference();
		}

		m_data = other.m_data;
		other.m_data = nullptr;
		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;

		MOVE_STACCK_TRACE(other,THIS_OBJECT)
	}

	return *this;
}
template<typename T>
bool lent_pointer_wrapper<T>::operator==(std::nullptr_t) const
{
	return m_data == nullptr;
}
template<typename T>
bool lent_pointer_wrapper<T>::operator!=(std::nullptr_t) const
{
	return m_data != nullptr;
}
template<typename T>
T* lent_pointer_wrapper<T>::operator->()
{
	return m_data;
}
template<typename T>
const T* lent_pointer_wrapper<T>::operator->() const
{
	return m_data;
}
template<typename T>
T& lent_pointer_wrapper<T>::operator*()
{
	if(m_data == nullptr)
	{
		throw bad_access_exception{ "Trying to dereference empty pointer" };
	}

	return *m_data;
}
template<typename T>
const T& lent_pointer_wrapper<T>::operator*() const
{
	if(m_data == nullptr)
	{
		throw bad_access_exception{ "Trying to dereference empty pointer" };
	}

	return *m_data;
}
template<typename T>
lent_pointer_wrapper<T>::operator bool() const
{
	return m_data != NULL;
}
template<typename T>
void lent_pointer_wrapper<T>::clear()
{
	UNREGISTER_STACK_TRACE(THIS_OBJECT)

	m_data = nullptr;

	if(m_refCounter)
	{
		m_refCounter->decrementLentReference();

		m_refCounter = nullptr;
	}
}
template<typename T>
T* lent_pointer_wrapper<T>::get()
{
	return m_data;
}
template<typename T>
const T* lent_pointer_wrapper<T>::get() const
{
	return m_data;
}
template<typename T>
T* lent_pointer_wrapper<T>::extract()
{
	UNREGISTER_STACK_TRACE(THIS_OBJECT)

	if(T* res = m_data)
	{
		m_data = nullptr;

		if(m_refCounter)
		{
			m_refCounter->decrementLentReference();

			m_refCounter = nullptr;
		}

		return res;
	}
	else
	{
		return nullptr;
	}
}
template<typename T>
typename lent_pointer_wrapper<T>::tagged_reference_counter lent_pointer_wrapper<T>::get_reference_counter() const
{
	return m_refCounter;
}

#endif

}
