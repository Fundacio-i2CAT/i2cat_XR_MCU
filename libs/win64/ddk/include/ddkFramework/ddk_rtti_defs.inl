
namespace ddk
{
namespace rtti
{

namespace detail
{

template<typename TT>
size_t ___get_type_id()
{
	static size_t s_id = 0;

	return s_id++;
}

template<typename T,typename TT>
size_t __get_type_id()
{
	static const size_t s_id = ___get_type_id<TT>();

	return s_id;
}

}

constexpr TypeInfo::TypeInfo(size_t i_id)
: m_id(i_id)
{
}
constexpr size_t TypeInfo::get_id() const
{
	return m_id;
}
constexpr bool TypeInfo::operator==(const TypeInfo& other) const
{
	return m_id == other.m_id;
}
constexpr bool TypeInfo::empty() const
{
	return m_id == s_invalid;
}

template<typename T,typename TT>
TypeInfo make_type_info()
{
	return detail::__get_type_id<T,TT>();
}
template<typename T>
TypeInfo make_type_info()
{
	return detail::___get_type_id<T>();
}

template<typename T>
const TypeInfo& type_info()
{
	static const TypeInfo s_typeInfo = make_type_info<T>();

	return s_typeInfo;
}

template<typename T,typename TT>
const TypeInfo& type_info()
{
	static const TypeInfo s_typeInfo = make_type_info<T,TT>();

	return s_typeInfo;
}

}
}
