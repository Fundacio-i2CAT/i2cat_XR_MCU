
namespace ddk
{

template<typename T>
weak_pointer_wrapper<T>::weak_pointer_wrapper(const weak_pointer_wrapper& other)
: m_data(other.m_data)
, m_refCounter(other.m_refCounter)
, m_deleter(other.m_deleter)
{
	if(m_refCounter)
	{
		m_refCounter->incrementWeakReference();
	}
}
template<typename T>
weak_pointer_wrapper<T>::weak_pointer_wrapper(weak_pointer_wrapper&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
, m_deleter(other.m_deleter)
{
	std::swap(m_data,other.m_data);
	std::swap(m_refCounter,other.m_refCounter);
}
template<typename T>
template<typename TT>
weak_pointer_wrapper<T>::weak_pointer_wrapper(const weak_pointer_wrapper<TT>& other)
: m_data(other.m_data)
, m_refCounter(other.m_refCounter)
, m_deleter(other.m_deleter)
{
	if(m_refCounter)
	{
		m_refCounter->incrementWeakReference();
	}
}
template<typename T>
template<typename TT>
weak_pointer_wrapper<T>::weak_pointer_wrapper(weak_pointer_wrapper<TT>&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
, m_deleter(other.m_deleter)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide a base class of T");

	if((m_data = other.m_data))
	{
		std::swap(m_refCounter,other.m_refCounter);
	}

	other.m_data = nullptr;
}
template<typename T>
weak_pointer_wrapper<T>::weak_pointer_wrapper(T* i_data,const tagged_reference_counter& i_refCounter,const tagged_pointer_deleter& i_deleter)
: m_data(i_data)
, m_refCounter(i_refCounter)
, m_deleter(i_deleter)
{
	if(m_refCounter)
	{
		m_refCounter->incrementWeakReference();
	}
}
template<typename T>
weak_pointer_wrapper<T>::~weak_pointer_wrapper()
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->decrementWeakReference());
	}
}
template<typename T>
weak_pointer_wrapper<T>& weak_pointer_wrapper<T>::operator=(std::nullptr_t)
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->decrementWeakReference());
	}

	return *this;
}
template<typename T>
weak_pointer_wrapper<T>& weak_pointer_wrapper<T>::operator=(const weak_pointer_wrapper& other)
{
	if(m_data != other.m_data)
	{
		m_data = other.m_data;

		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->decrementWeakReference());
		}

		if((m_refCounter = other.m_refCounter))
		{
			m_refCounter->incrementWeakReference();
		}

		m_deleter = other.m_deleter;
	}

	return *this;
}
template<typename T>
weak_pointer_wrapper<T>& weak_pointer_wrapper<T>::operator=(weak_pointer_wrapper&& other)
{
	if(m_data != other.m_data)
	{
		m_data = other.m_data;
		other.m_data = nullptr;

		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->decrementWeakReference());
		}

		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;

		m_deleter = other.m_deleter;
		other.m_deleter = nullptr;
	}

	return *this;
}
template<typename T>
template<typename TT>
weak_pointer_wrapper<T>& weak_pointer_wrapper<T>::operator=(const weak_pointer_wrapper<TT>& other)
{
	if(m_data != other.m_data)
	{
		m_data = other.m_data;

		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->decrementWeakReference());
		}

		if((m_refCounter = other.m_refCounter))
		{
			m_refCounter->incrementWeakReference();
		}

		m_deleter = other.m_deleter;
	}

	return *this;
}
template<typename T>
template<typename TT>
weak_pointer_wrapper<T>& weak_pointer_wrapper<T>::operator=(weak_pointer_wrapper<TT>&& other)
{
	if(m_data != other.m_data)
	{
		m_data = other.m_data;
		other.m_data = nullptr;

		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->decrementWeakReference());
		}

		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;

		m_deleter = other.m_deleter;
		other.m_deleter = nullptr;
	}

	return *this;
}
template<typename T>
bool weak_pointer_wrapper<T>::operator==(const std::nullptr_t&) const
{
	return (m_refCounter && m_refCounter->hasSharedReferences());
}
template<typename T>
bool weak_pointer_wrapper<T>::operator!=(const std::nullptr_t&) const
{
	return (m_refCounter && m_refCounter->hasSharedReferences());
}
template<typename T>
shared_pointer_wrapper<T> weak_pointer_wrapper<T>::share() const
{
	return (m_refCounter && m_refCounter->incrementSharedReferenceIfNonEmpty()) ? shared_pointer_wrapper<T>(m_data,m_refCounter,m_deleter,true) : shared_pointer_wrapper<T>(nullptr);
}
template<typename T>
void weak_pointer_wrapper<T>::clearIfCounterVoid(size_t i_numWeakRefs)
{
	if(i_numWeakRefs == 0)
	{
		shared_reference_counter* refCounter = m_refCounter.extract_pointer();

		::delete(refCounter);
	}
}

}
