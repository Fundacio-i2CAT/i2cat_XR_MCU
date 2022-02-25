
namespace ddk
{
namespace detail
{

template<typename Map,typename Node>
void avl_balancer<Map,Node>::re_parent(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> node,unique_pointer_wrapper<Node> newNode)
{
    if(lent_pointer_wrapper<Node> nodeParent = node->get_parent())
    {
        newNode->set_parent(nodeParent);

        if(nodeParent->get_left_child() == node)
        {
            nodeParent->set_left_child(std::move(newNode));
        }
        else
        {
            nodeParent->set_right_child(std::move(newNode));
        }
    }
    else
    {
        newNode->set_parent(nullptr);

        fromMap->m_root = std::move(newNode);
    }
}
template<typename Map,typename Node>
void avl_balancer<Map,Node>::set_to_left(lent_reference_wrapper<Node> node,unique_pointer_wrapper<Node> newNode)
{
    if(newNode)
    {
        newNode->set_parent(node);

        node->set_left_child(std::move(newNode));
    }
    else
    {
        node->set_left_child(nullptr);
    }
}
template<typename Map,typename Node>
void avl_balancer<Map,Node>::set_to_right(lent_reference_wrapper<Node> node,unique_pointer_wrapper<Node> newNode)
{
    if(newNode)
    {
        newNode->set_parent(node);

        node->set_right_child(std::move(newNode));
    }
    else
    {
        node->set_right_child(nullptr);
    }
}
template<typename Map,typename Node>
lent_reference_wrapper<Node> avl_balancer<Map,Node>::balance_simple_to_left(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode)
{
    unique_pointer_wrapper<Node> acquiredRightChild = fromNode->extract_right_child();
    lent_pointer_wrapper<Node> acquiredRightChildRef = lend(acquiredRightChild);

    if(acquiredRightChild)
    {
        unique_reference_wrapper<Node> acquiredFromNode = fromNode->extract_node(fromMap);
        unique_pointer_wrapper<Node> rightLeftChild = acquiredRightChild->extract_left_child();

        acquiredRightChild->set_height(0);
        acquiredFromNode->set_height(0);

        re_parent(fromMap,fromNode,std::move(acquiredRightChild));

        set_to_left(promote_to_ref(acquiredRightChildRef),std::move(acquiredFromNode));

        set_to_right(fromNode,std::move(rightLeftChild));
    }

    return (acquiredRightChildRef) ? promote_to_ref(acquiredRightChildRef) : fromNode;
}
template<typename Map,typename Node>
lent_reference_wrapper<Node> avl_balancer<Map,Node>::balance_simple_to_right(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode)
{
    unique_pointer_wrapper<Node> acquiredLeftChild = fromNode->extract_left_child();
    lent_pointer_wrapper<Node> acquiredLeftChildRef = lend(acquiredLeftChild);

    //the left child is going to be the parent of the current node
    if(acquiredLeftChild)
    {
        unique_reference_wrapper<Node> acquiredFromNode = fromNode->extract_node(fromMap);
        unique_pointer_wrapper<Node> leftRightChild = acquiredLeftChild->extract_right_child();

        acquiredLeftChild->set_height(0);
        acquiredFromNode->set_height(0);

        re_parent(fromMap,fromNode,std::move(acquiredLeftChild));

        set_to_right(promote_to_ref(acquiredLeftChildRef),std::move(acquiredFromNode));

        set_to_left(fromNode,std::move(leftRightChild));
    }

    return (acquiredLeftChildRef) ? promote_to_ref(acquiredLeftChildRef) : fromNode;
}
template<typename Map,typename Node>
lent_reference_wrapper<Node> avl_balancer<Map,Node>::balance_double_to_left(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode)
{
    unique_pointer_wrapper<Node> acquiredRightChild = fromNode->extract_right_child();
    lent_pointer_wrapper<Node> acquiredRightChildRef = lend(acquiredRightChild);
    unique_pointer_wrapper<Node> acquiredRightLeftChild = (acquiredRightChildRef) ? acquiredRightChildRef->extract_left_child() : nullptr;
    lent_pointer_wrapper<Node> acquiredRightLeftChildRef = lend(acquiredRightLeftChild);

    //the left child is going to be the parent of the current node

    if(acquiredRightChild)
    {
        unique_reference_wrapper<Node> acquiredFromNode = fromNode->extract_node(fromMap);
        unique_pointer_wrapper<Node> rightLeftRightChild = (acquiredRightLeftChild) ? acquiredRightLeftChild->extract_right_child() : nullptr;
        unique_pointer_wrapper<Node> rightLeftLeftChild = (acquiredRightLeftChild) ? acquiredRightLeftChild->extract_left_child() : nullptr;

        acquiredRightChildRef->set_height(0);

        set_to_left(promote_to_ref(acquiredRightChildRef),std::move(rightLeftRightChild));

        if(acquiredRightLeftChild)
        {
            acquiredRightLeftChild->set_height(0);

            re_parent(fromMap,fromNode,std::move(acquiredRightLeftChild));

            set_to_right(promote_to_ref(acquiredRightLeftChildRef),std::move(acquiredRightChild));

            set_to_left(promote_to_ref(acquiredRightLeftChildRef),std::move(acquiredFromNode));
        }

        fromNode->set_height(0);

        set_to_right(fromNode,std::move(rightLeftLeftChild));
    }

    return (acquiredRightLeftChildRef) ? promote_to_ref(acquiredRightLeftChildRef) : fromNode;
}
template<typename Map,typename Node>
lent_reference_wrapper<Node> avl_balancer<Map,Node>::balance_double_to_right(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode)
{
    unique_pointer_wrapper<Node> acquiredLeftChild = fromNode->extract_left_child();
    lent_pointer_wrapper<Node> acquiredLeftChildRef = lend(acquiredLeftChild);
    unique_pointer_wrapper<Node> acquiredLeftRightChild = (acquiredLeftChildRef) ? acquiredLeftChildRef->extract_right_child() : nullptr;
    lent_pointer_wrapper<Node> acquiredLeftRightChildRef = lend(acquiredLeftRightChild);

    //the left child is going to be the parent of the current node

    if(acquiredLeftChild)
    {
        unique_reference_wrapper<Node> acquiredFromNode = fromNode->extract_node(fromMap);
        unique_pointer_wrapper<Node> leftRightLeftChild = (acquiredLeftRightChild) ? acquiredLeftRightChild->extract_left_child() : nullptr;
        unique_pointer_wrapper<Node> leftRightRightChild = (acquiredLeftRightChild) ? acquiredLeftRightChild->extract_right_child() : nullptr;

        acquiredLeftChildRef->set_height(0);

        set_to_right(promote_to_ref(acquiredLeftChildRef),std::move(leftRightLeftChild));

        if(acquiredLeftRightChild)
        {
            acquiredLeftRightChild->set_height(0);

            re_parent(fromMap,fromNode,std::move(acquiredLeftRightChild));

            set_to_left(promote_to_ref(acquiredLeftRightChildRef),std::move(acquiredLeftChild));

            set_to_right(promote_to_ref(acquiredLeftRightChildRef),std::move(acquiredFromNode));
        }

        fromNode->set_height(0);

        set_to_left(fromNode,std::move(leftRightRightChild));
    }

    return (acquiredLeftRightChildRef) ? promote_to_ref(acquiredLeftRightChildRef) : fromNode;
}
template<typename Map,typename Node>
unique_reference_wrapper<Node> avl_balancer<Map,Node>::balance(lent_reference_wrapper<Map> fromMap, lent_reference_wrapper<Node> fromNode,bool heightIncrease)
{
    if(lent_pointer_wrapper<Node> thisParent = fromNode->get_parent())
    {
        lent_pointer_wrapper<Node> localRoot = thisParent;
        lent_pointer_wrapper<Node> leftChild = thisParent->get_left_child();
        lent_pointer_wrapper<Node> rightChild = thisParent->get_right_child();

        if(leftChild == fromNode)
        {
            if(heightIncrease)
            {
                if(thisParent->get_height() == -1)
                {
                    thisParent->set_height(0);
                    heightIncrease = false;
                }
                else if(thisParent->get_height() == 0)
                {
                    thisParent->set_height(1);
                    heightIncrease = true;
                }
                else if(thisParent->get_height() == 1)
                {
                    if(leftChild->get_height() == 1)
                    {
                        localRoot = balance_simple_to_right(fromMap,promote_to_ref(thisParent));
                    }
                    else
                    {
                        localRoot = balance_double_to_right(fromMap,promote_to_ref(thisParent));
                    }
                    heightIncrease = false;
                }
            }
        }
        else if(rightChild == fromNode)
        {
            if(heightIncrease)
            {
                if(thisParent->get_height() == -1)
                {
                    if(rightChild->get_height() == 1)
                    {
                        localRoot = balance_double_to_left(fromMap,promote_to_ref(thisParent));
                    }
                    else
                    {
                        localRoot = balance_simple_to_left(fromMap,promote_to_ref(thisParent));
                    }
                    heightIncrease = false;
                }
                else if(thisParent->get_height() == 0)
                {
                    thisParent->set_height(-1);
                    heightIncrease = true;
                }
                else if(thisParent->get_height() == 1)
                {
                    thisParent->set_height(0);
                    heightIncrease = false;
                }
            }
        }

        //forward the action to its parent
        return balance(fromMap,promote_to_ref(localRoot),heightIncrease);
    }
    else
    {
        return fromNode->extract_node(fromMap);
    }
}
template<typename Map,typename Node>
unique_reference_wrapper<Node> avl_balancer<Map,Node>::balance(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode)
{
    return balance(fromMap,fromNode,true);
}

}
}