
namespace ddk
{

template<typename ... Types, typename Allocator>
filter_function<bool(Types...),Allocator>::filter_function(const function<bool(Types...),Allocator>& other)
: function<bool(Types...),Allocator>(other)
{
}

}
