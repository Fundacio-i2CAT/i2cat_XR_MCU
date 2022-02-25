
namespace ddk
{
namespace detail
{

template<typename Map,typename Node>
unique_reference_wrapper<Node> rb_balancer<Map,Node>::balance(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode)
{
    return promote_to_ref(std::move(fromMap->m_root));
}

}
}