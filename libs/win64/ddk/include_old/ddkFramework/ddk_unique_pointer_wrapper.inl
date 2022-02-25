
namespace ddk
{

template<typename T>
unique_pointer_wrapper<T>::unique_pointer_wrapper()
: m_data(nullptr)
, m_refCounter(nullptr)
, m_deleter(nullptr)
{
}
template<typename T>
unique_pointer_wrapper<T>::unique_pointer_wrapper(std::nullptr_t)
: m_data(nullptr)
, m_refCounter(nullptr)
, m_deleter(nullptr)
{
}
template<typename T>
unique_pointer_wrapper<T>::unique_pointer_wrapper(unique_pointer_wrapper&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
, m_deleter(other.m_deleter)
{
	std::swap(m_data,other.m_data);
	std::swap(m_refCounter,other.m_refCounter);
}
template<typename T>
template<typename TT>
unique_pointer_wrapper<T>::unique_pointer_wrapper(unique_pointer_wrapper<TT>&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
, m_deleter(other.m_deleter)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide inherited classes");

	if((m_data = other.m_data))
	{
		std::swap(m_refCounter,other.m_refCounter);
	}

	other.m_data = nullptr;
}
template<typename T>
unique_pointer_wrapper<T>::~unique_pointer_wrapper()
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->removeStrongReference());
	}
}
template<typename T>
unique_pointer_wrapper<T>& unique_pointer_wrapper<T>::operator=(std::nullptr_t)
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->removeStrongReference());
	}

	return *this;
}
template<typename T>
unique_pointer_wrapper<T>& unique_pointer_wrapper<T>::operator=(unique_pointer_wrapper&& other)
{
	if(m_data != other.m_data)
	{
		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->removeStrongReference());
		}

		m_deleter = other.m_deleter;
		m_data = other.m_data;
		other.m_data = nullptr;
		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;
	}

	return *this;
}
template<typename T>
template<typename TT>
unique_pointer_wrapper<T>& unique_pointer_wrapper<T>::operator=(unique_pointer_wrapper<TT>&& other)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide inherited classes");

	if(m_data != static_cast<const T*>(other.m_data))
	{
		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->removeStrongReference());
		}

		if((m_data = other.m_data))
		{
			m_deleter = other.m_deleter;
		}

		other.m_data = nullptr;
		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;
	}

	return *this;
}
template<typename T>
bool unique_pointer_wrapper<T>::operator==(std::nullptr_t) const
{
	return m_data == nullptr;
}
template<typename T>
bool unique_pointer_wrapper<T>::operator!=(std::nullptr_t) const
{
	return m_data != nullptr;
}
template<typename T>
T* unique_pointer_wrapper<T>::operator->()
{
	return m_data;
}
template<typename T>
const T* unique_pointer_wrapper<T>::operator->() const
{
	return m_data;
}
template<typename T>
T& unique_pointer_wrapper<T>::operator*()
{
	if(m_data == nullptr)
	{
		MAKE_IT_CRASH
	}

	return *m_data;
}
template<typename T>
const T& unique_pointer_wrapper<T>::operator*() const
{
	if(m_data == nullptr)
	{
		MAKE_IT_CRASH
	}

	return *m_data;
}
template<typename T>
unique_pointer_wrapper<T>::operator bool() const
{
	return m_data != nullptr;
}
template<typename T>
void unique_pointer_wrapper<T>::clear()
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->removeStrongReference());
	}
}
template<typename T>
T* unique_pointer_wrapper<T>::get()
{
	return m_data;
}
template<typename T>
const T* unique_pointer_wrapper<T>::get() const
{
	return m_data;
}
template<typename T>
bool unique_pointer_wrapper<T>::empty() const
{
	return m_data == nullptr;
}
template<typename T>
const tagged_pointer_deleter& unique_pointer_wrapper<T>::get_deleter() const
{
	return m_deleter;
}
template<typename T>
unique_pointer_wrapper<T>::unique_pointer_wrapper(T* i_data,const tagged_reference_counter& i_refCounter,const tagged_pointer_deleter& i_refDeleter)
: m_data(i_data)
, m_refCounter(i_refCounter)
, m_deleter(i_refDeleter)
{
	m_refCounter->addStrongReference();
}
template<typename T>
unique_pointer_wrapper<T>::unique_pointer_wrapper(T* i_data,tagged_pointer<unique_reference_counter>&& i_refCounter,const tagged_pointer_deleter& i_refDeleter)
: m_data(i_data)
, m_refCounter(std::move(i_refCounter))
, m_deleter(i_refDeleter)
{
}
template<typename T>
void unique_pointer_wrapper<T>::clearIfCounterVoid(bool i_hasRefs)
{
	if(i_hasRefs == false)
	{
		DDK_ASSERT(m_refCounter->hasLentReferences() == false,"Still lent references alive while destroying unique reference");

		const short tagCategory = m_refCounter.get_tag();
		unique_reference_counter* refCounter = m_refCounter.extract_pointer();

		if(tagCategory == ReferenceAllocationType::Dynamic)
		{
			delete refCounter;
		}
		else if(tagCategory == ReferenceAllocationType::Contiguous)
		{
			refCounter->~unique_reference_counter();
		}

		if(const resource_deleter_interface* deleter = m_deleter.get_pointer())
		{
			const short allocCategory = m_deleter.get_tag();

			//before destructing, resolve address
			const void* ptrBase = allocator_address_reference_wrapper(m_data);

			if(allocCategory == AllocationMode::AllocationOnly)
			{
				m_data->~T();
			}

			deleter->deallocate(ptrBase);
		}
		else
		{
			::delete(m_data);
		}
	}

	m_refCounter = nullptr;
	m_data = nullptr;
	m_deleter = nullptr;
}
template<typename T>
unique_reference_counter* unique_pointer_wrapper<T>::extract_reference_counter()
{
	return m_refCounter.extract_pointer();
}

}