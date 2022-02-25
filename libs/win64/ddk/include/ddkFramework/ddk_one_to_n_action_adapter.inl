
namespace ddk
{
namespace iter
{

template<typename Action>
template<size_t ... Indexs>
high_order_array<Action,mpl::num_ranks<Indexs...>> one_to_n_diagonal_action_adapter<Action>::resolve(const Action& i_action) const
{
	high_order_array<Action,mpl::num_ranks<Indexs...>> res;

	//since this loop is defined by constexpr constrains, we may assume the compiler will unroll it
	for(size_t index = 0;index<mpl::num_ranks<Indexs...>;++index)
	{
		res[index] = i_action;
	}

	return res;
}

}
}