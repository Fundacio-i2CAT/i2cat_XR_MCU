#pragma once

#include "ddk_high_order_array.h"

namespace ddk
{
namespace iter
{

template<typename Action>
class one_to_n_diagonal_action_adapter
{
public:
	template<size_t ... Indexs>
	high_order_array<Action,mpl::num_ranks<Indexs...>> resolve(const Action& i_action) const;
};

}
}

#include "ddk_one_to_n_action_adapter.inl"