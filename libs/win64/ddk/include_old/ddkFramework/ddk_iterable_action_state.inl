
namespace ddk
{

template<typename T>
void action_result::emplace(T&& i_res)
{
	m_result = i_res;
}
template<typename T>
typename embedded_type<T>::ref_type action_result::get_as()
{
	return m_result.template get_as<T>();
}
template<typename T>
typename embedded_type<T>::cref_type action_result::get_as() const
{
	return m_result.template get_as<T>();
}
template<typename T>
typename embedded_type<T>::rref_type action_result::extract_as() &&
{
	return std::move(m_result).template extract<T>();
}
template<typename T>
bool action_result::is_base_of() const
{
	return m_result.template is_base_of<T>();
}

template<typename T>
void action_state::forward_result(T&& i_result)
{
	m_actionResult.emplace(std::forward<T>(i_result));
}
template<typename T>
T&& action_state::forward_result()
{
    return std::move(m_actionResult).template extract_as<T>();
}
template<typename Visitor>
typename Visitor::return_type action_state::visit(Visitor& i_visitor) const
{
	if constexpr(std::is_same<void,typename Visitor::return_type>::value)
	{
		m_actionResult.error().visit(i_visitor);
	}
	else
	{
		return m_actionResult.error().visit(i_visitor);
	}
}

}
