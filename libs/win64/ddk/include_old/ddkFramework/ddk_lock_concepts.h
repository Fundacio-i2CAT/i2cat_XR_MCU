#pragma once

#define IS_LOCKABLE(_TYPE) \
    typename std::enable_if<ddk::concepts::is_lockable<_TYPE>>::type

namespace ddk
{
namespace concepts
{

template<typename T>
struct is_lockable_type
{
private:
    template<typename TT>
    static std::true_type resolve(TT&, typename std::add_pointer<decltype(std::declval<TT>().lock())>::type,
                 typename std::add_pointer<decltype(std::declval<TT>().try_lock())>::type,
                 typename std::add_pointer<decltype(std::declval<TT>().unlock())>::type);
    template<typename TT>
    static std::false_type resolve(TT&, ...);

public:
    static const bool value = decltype(resolve(std::declval<T&>(),nullptr,nullptr,nullptr))::value;
};

template<typename T>
inline constexpr bool is_lockable = is_lockable_type<T>::value;

}
}
