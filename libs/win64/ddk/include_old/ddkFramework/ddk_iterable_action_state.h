#pragma once

#include "ddk_iterable_action.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_variant.h"

namespace ddk
{

typedef error<ActionError,EraseActionError,AddActionError,ShiftActionError> action_error;

class action_result
{
public:
	typedef variant<action_error,stop_action,erase_action,add_action,go_forward_action,go_backward_action,shift_action> nested_result;

	action_result() = default;

	template<typename T>
	void emplace(T&& i_res);
	const nested_result& get() const;
	template<typename T>
	inline typename embedded_type<T>::cref_type get_as() const;
	template<typename T>
	inline typename embedded_type<T>::ref_type get_as();
	template<typename T>
	inline typename embedded_type<T>::rref_type extract_as() &&;
	const action_error& error() const;
	bool operator==(const result_success_t&) const;
	bool operator!=(const result_success_t&) const;
	bool operator==(const action_error&) const ;
	bool operator!=(const action_error&) const ;
	operator bool() const;
	template<typename T>
	inline bool is_base_of() const;

private:
	nested_result m_result = go_no_place;
};

class action_state
{
public:
	action_state() = default;
	~action_state() = default;

	template<typename T>
	void forward_result(T&& i_result);
	template<typename T>
	T&& forward_result();
	action_result get() const;
	template<typename Visitor>
	typename Visitor::return_type visit(Visitor& i_visitor) const;
	bool operator==(result_success_t) const;
	bool operator!=(result_success_t) const;
	bool operator==(const action_error& i_error) const;
	bool operator!=(const action_error& i_error) const;

private:
	action_result m_actionResult;
};

typedef lent_reference_wrapper<action_state> action_state_lent_ref;
typedef lent_reference_wrapper<const action_state> action_state_const_lent_ref;
typedef lent_pointer_wrapper<action_state> action_state_lent_ptr;
typedef lent_pointer_wrapper<const action_state> action_state_const_lent_ptr;

}

#include "ddk_iterable_action_state.inl"
