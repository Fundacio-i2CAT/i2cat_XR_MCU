
namespace ddk
{

template<typename Return>
template<typename T>
Return static_visitor<Return>::operator()(const T&) const
{
	static_assert(sizeof(T) == 0,"You shall provide visit method for this type");

	if constexpr(std::is_same<void,Return>::value == false)
	{
		return ddk::crash_on_return<Return>::value();
	}
}

}