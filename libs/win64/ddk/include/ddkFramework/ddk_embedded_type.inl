
namespace ddk
{

template<typename T>
constexpr embedded_type<T&>::embedded_type(internal_type& other)
: m_data(other)
{
}
template<typename T>
constexpr embedded_type<T&>::embedded_type(const embedded_type<T&>& other)
: m_data(other.m_data)
{
}
template<typename T>
template<typename Type>
embedded_type<T&>& embedded_type<T&>::operator=(Type&& val)
{
	new(&m_data)embedded_type(std::forward<Type>(val));

	return *this;
}
template<typename T>
constexpr bool embedded_type<T&>::operator==(const embedded_type<T&>& other) const
{
	return m_data == other.m_data;
}
template<typename T>
typename embedded_type<T&>::rref_type embedded_type<T&>::extract()
{
	return m_data;
}
template<typename T>
constexpr typename embedded_type<T&>::cref_type embedded_type<T&>::get() const
{
	return m_data;
}
template<typename T>
constexpr typename embedded_type<T&>::ref_type embedded_type<T&>::get()
{
	return m_data;
}
template<typename T>
constexpr typename embedded_type<T&>::pointer_type embedded_type<T&>::get_ptr()
{
	return &m_data;
}
template<typename T>
constexpr typename embedded_type<T&>::cref_type embedded_type<T&>::operator*() const
{
	return m_data;
}
template<typename T>
constexpr typename embedded_type<T&>::ref_type embedded_type<T&>::operator*()
{
	return m_data;
}
template<typename T>
constexpr typename embedded_type<T&>::pointer_type embedded_type<T&>::operator->()
{
	return &m_data;
}
template<typename T>
constexpr typename embedded_type<T&>::cpointer_type embedded_type<T&>::operator->() const
{
	return &m_data;
}
template<typename T>
constexpr embedded_type<T&>::operator ref_type()
{
	return m_data;
}
template<typename T>
constexpr embedded_type<T&>::operator cref_type() const
{
	return m_data;
}
template<typename T>
void embedded_type<T&>::inplace_construct(T& val)
{
	new(this) embedded_type(val);
}
template<typename T>
void embedded_type<T&>::inplace_assign(T& val)
{
	//references must be reconstructed every time
	new(this) embedded_type(val);
}
template<typename T>
void embedded_type<T&>::inplace_destroy()
{
	//in references nothing needs to be done
}
template<typename T>
bool embedded_type<T&>::construct(void* address,T& val)
{
	return new(address)embedded_type(val) != nullptr;
}
template<typename T>
bool embedded_type<T&>::assign(void* address,T& val)
{
	//references must be reconstructed every time
	return new(address)embedded_type(val) != nullptr;
}
template<typename T>
bool embedded_type<T&>::destroy(void* address)
{
	//in references nothing needs to be done
	return true;
}

template<typename T>
constexpr embedded_type<T&&>::embedded_type(T&& other)
: m_data(std::move(other))
{
}
template<typename T>
constexpr embedded_type<T&&>::embedded_type(embedded_type&& other)
: m_data(std::move(other.m_data))
{
}
template<typename T>
constexpr bool embedded_type<T&&>::operator==(const embedded_type<T&&>& other) const
{
	return m_data == other.m_data;
}
template<typename T>
constexpr typename embedded_type<T&&>::cref_type embedded_type<T&&>::get() const
{
	return std::move(m_data);
}
template<typename T>
constexpr typename embedded_type<T&&>::ref_type embedded_type<T&&>::get()
{
	return std::move(m_data);
}
template<typename T>
constexpr typename embedded_type<T&&>::pointer_type embedded_type<T&&>::get_ptr()
{
	return &m_data;
}
template<typename T>
typename embedded_type<T&&>::rref_type embedded_type<T&&>::extract()
{
	return std::move(m_data);
}
template<typename T>
constexpr typename embedded_type<T&&>::cref_type embedded_type<T&&>::operator*() const
{
	return m_data;
}
template<typename T>
constexpr typename embedded_type<T&&>::ref_type embedded_type<T&&>::operator*()
{
	return m_data;
}
template<typename T>
constexpr typename embedded_type<T&&>::pointer_type embedded_type<T&&>::operator->()
{
	return &m_data;
}
template<typename T>
constexpr typename embedded_type<T&&>::cpointer_type embedded_type<T&&>::operator->() const
{
	return &m_data;
}
template<typename T>
constexpr embedded_type<T&&>::operator ref_type()
{
	return m_data;
}
template<typename T>
constexpr embedded_type<T&&>::operator cref_type() const
{
	return m_data;
}
template<typename T>
void embedded_type<T&&>::inplace_construct(T&& val)
{
	new(this) embedded_type(std::move(val));
}
template<typename T>
void embedded_type<T&&>::inplace_assign(T&& val)
{
	//references must be reconstructed every time
	new(this) embedded_type(std::move(val));
}
template<typename T>
void embedded_type<T&&>::inplace_destroy()
{
	//in references nothing needs to be done
}
template<typename T>
bool embedded_type<T&&>::construct(void* address,T&& val)
{
	return new(address) embedded_type(std::move(val)) != nullptr;
}
template<typename T>
bool embedded_type<T&&>::destroy(void* address)
{
	//in references nothing needs to be done
	return true;
}
template<typename T>
bool embedded_type<T&&>::assign(void* address,T&& val)
{
	//references must be reconstructed every time

	return construct(address,std::move(val));
}
template<typename T>
bool embedded_type<T&&>::swap(void* addressA,internal_type&& valA,void* addressB,internal_type&& valB)
{
	internal_type&& tmp = std::move(valA);

	return construct(addressA,std::move(valB)) && construct(addressB,std::move(tmp));
}

template<typename T>
constexpr embedded_type<T>::embedded_type(const embedded_type<T>& other)
: m_data(other.m_data)
{
}
template<typename T>
constexpr embedded_type<T>::embedded_type(embedded_type<T>&& other)
: m_data(std::move(other.m_data))
{
}
template<typename T>
template<typename Arg,typename ... Args>
constexpr embedded_type<T>::embedded_type(Arg&& i_arg,Args&& ... i_args)
: m_data(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...)
{
}
template<typename T>
embedded_type<T>& embedded_type<T>::operator=(const internal_type& other)
{
	m_data = other;

	return *this;
}
template<typename T>
embedded_type<T>& embedded_type<T>::operator=(internal_type&& other)
{
	m_data = std::move(other);

	return *this;
}
template<typename T>
embedded_type<T>& embedded_type<T>::operator=(const embedded_type<T>& other)
{
	m_data = other.m_data;

	return *this;
}
template<typename T>
embedded_type<T>& embedded_type<T>::operator=(embedded_type<T>&& other)
{
	m_data = std::move(other.m_data);

	return *this;
}
template<typename T>
constexpr bool embedded_type<T>::operator==(const embedded_type<T>& other) const
{
	return m_data == other.m_data;
}
template<typename T>
constexpr typename embedded_type<T>::cref_type embedded_type<T>::get() const
{
	return std::forward<embedded_type<T>::cref_type>(m_data);
}
template<typename T>
constexpr typename embedded_type<T>::ref_type embedded_type<T>::get()
{
	return std::forward<embedded_type<T>::ref_type>(m_data);
}
template<typename T>
constexpr typename embedded_type<T>::cpointer_type embedded_type<T>::get_ptr() const
{
	return &m_data;
}
template<typename T>
constexpr typename embedded_type<T>::pointer_type embedded_type<T>::get_ptr()
{
	return &m_data;
}
template<typename T>
inline typename embedded_type<T>::rref_type embedded_type<T>::extract()
{
	return std::forward<embedded_type<T>::rref_type>(m_data);
}
template<typename T>
constexpr typename embedded_type<T>::cref_type embedded_type<T>::operator*() const
{
	return std::forward<embedded_type<T>::cref_type>(m_data);
}
template<typename T>
constexpr typename embedded_type<T>::ref_type embedded_type<T>::operator*()
{
	return std::forward<embedded_type<T>::ref_type>(m_data);
}
template<typename T>
constexpr typename embedded_type<T>::pointer_type embedded_type<T>::operator->()
{
	return &m_data;
}
template<typename T>
constexpr typename embedded_type<T>::cpointer_type embedded_type<T>::operator->() const
{
	return &m_data;
}
template<typename T>
embedded_type<T>::operator rref_type() &&
{
	return std::move(m_data);
}
template<typename T>
template<typename ... Args>
void embedded_type<T>::inplace_construct(Args&& ... i_args)
{
	new(this) embedded_type{ std::forward<Args>(i_args)... };
}
template<typename T>
template<typename ... Args>
constexpr void embedded_type<T>::inplace_assign(Args&& ... i_args)
{
	T obj{ std::forward<Args>(i_args)... };

	if constexpr (std::is_copy_assignable<T>::value)
	{
		m_data = obj;
	}
	else
	{
		m_data = std::move(obj);
	}
}
template<typename T>
void embedded_type<T>::inplace_destroy()
{
	m_data.~T();
}
template<typename T>
template<typename ... Args>
bool embedded_type<T>::construct(void* address,Args&& ... i_args)
{
	return new(address) embedded_type(std::forward<Args>(i_args) ...) != nullptr;
}
template<typename T>
bool embedded_type<T>::destroy(void* address)
{
	embedded_type<T>* _data = reinterpret_cast<embedded_type<T>*>(address);

	_data->m_data.~T();

	return true;
}
template<typename T>
template<typename Type>
bool embedded_type<T>::assign(void* address,Type&& val)
{
	reinterpret_cast<embedded_type<T>*>(address)->m_data = std::forward<Type>(val);

	return true;
}

}