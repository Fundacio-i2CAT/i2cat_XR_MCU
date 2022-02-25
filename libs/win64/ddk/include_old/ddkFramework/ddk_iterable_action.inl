
#include "ddk_variant.h"
#include "ddk_scoped_enum.h"
#include "ddk_result.h"

namespace ddk
{

SCOPED_ENUM_DECL(EraseActionError,
                 NonExistingValue,
                 ErasingFromConstantIterable);
typedef ddk::result<void,EraseActionError> erase_result;

SCOPED_ENUM_DECL(AddActionError,
                NonConvertibleType,
                AddingToConstantIterable,
                AddedItemFiltered);
typedef ddk::result<void,AddActionError> add_result;

SCOPED_ENUM_DECL(ActionError,
                 RemovalError,
                 AdditionError,
                 ShiftError);

typedef result<void,ShiftActionError> shift_result;

typedef variant<erase_action,add_action,go_forward_action> input_action;
typedef variant<erase_action,add_action,go_forward_action> output_action;
typedef variant<erase_action,add_action,go_forward_action> forward_action;
typedef variant<erase_action,add_action,go_forward_action,go_backward_action> bidirectional_action;
typedef variant<erase_action,add_action,go_forward_action,go_backward_action,shift_action> random_access_action;

typedef variant<go_forward_action> const_input_action;
typedef variant<go_forward_action> const_output_action;
typedef variant<go_forward_action> const_forward_action;
typedef variant<go_forward_action,go_backward_action> const_bidirectional_action;
typedef variant<shift_action> const_random_access_action;

bool operator==(const base_action& i_lhs,const base_action& i_rhs);
bool operator!=(const base_action& i_lhs,const base_action& i_rhs);
bool operator==(const shift_action& i_lhs,const shift_action& i_rhs);
bool operator!=(const shift_action& i_lhs,const shift_action& i_rhs);

const extern stop_action stop_iteration;
const extern erase_action erase_place;
const extern add_action add_place;
const extern go_forward_action go_next_place;
const extern go_backward_action go_prev_place;
const extern shift_action go_to_place;
const extern shift_action go_no_place;

}