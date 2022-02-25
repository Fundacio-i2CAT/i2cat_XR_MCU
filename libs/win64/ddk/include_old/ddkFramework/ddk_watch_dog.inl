
template<typename T>
ddk::watch_dog watch(const T&)
{
	static_assert(sizeof(T) == 0,"Type not watchable");

	return ddk::crash_on_return<ddk::watch_dog>::value();
}
template<typename T>
bool is_watchable(const T&)
{
	return false;
}
template<typename T>
bool is_watched(const T&)
{
	return false;
}
