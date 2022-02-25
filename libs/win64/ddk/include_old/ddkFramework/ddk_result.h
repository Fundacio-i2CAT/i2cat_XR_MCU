#pragma once

#include "ddk_variant.h"
#include "ddk_optional.h"
#include "ddk_class_rules.h"

#if defined(DDK_DEBUG)

#define SET_CHECKED(_RESULT) \
	set_checked(_RESULT);

#else

#define SET_CHECKED(_RESULT)

#endif

namespace ddk
{

struct result_success_t{};
const result_success_t success = result_success_t();

template<typename,typename>
class result;

template<typename Error>
class result<void,Error>
{
#if defined(DDK_DEBUG)
	friend inline void set_checked(const result& i_result)
	{
		i_result.m_checked = true;
	}
#endif

	friend std::ostringstream& operator<<(std::ostringstream& o_stream, const result<void,Error>& i_result)
	{
		if (i_result.hasError())
		{
			o_stream << "Error: " << i_result.getError();
		}
		else
		{
			o_stream << "Success";
		}

		return o_stream;
	}
	friend std::string operator<<(const std::string& i_str, const result<void,Error>& i_result)
	{
		std::string res;

		if (i_result.hasError())
		{
			res = "Error: " << i_result.getError();
		}
		else
		{
			res = "Success";
		}

		return res;
	}

public:
	typedef void payload_t;
	typedef Error error_t;

	result(const result_success_t&);
	result(const Error& i_error);
	result(const result& other);
	result(result&& other);
	~result();
	result& operator=(const result& other);
	result& operator=(result&& other);
	Error error() const;
	explicit operator bool() const;
	result& dismiss();
	const result& dismiss() const;
	bool operator==(const result_success_t&) const;
	bool operator!=(const result_success_t&) const;
	bool operator==(const Error& i_error) const;
	bool operator!=(const Error& i_error) const;

private:
	optional<Error> m_nestedRes;

#if defined(DDK_DEBUG)
	mutable bool m_checked = false;
#endif
};

template<typename T, typename Error>
class result : contravariant_rules<T>
{
#if defined(DDK_DEBUG)
	friend inline void set_checked(const result& i_result)
	{
		i_result.m_checked = true;
	}
#endif

	friend std::ostringstream& operator<<(std::ostringstream& o_stream, const result<T,Error>& i_result)
	{
		if (i_result.hasError())
		{
			o_stream << "Error: " << i_result.getError();
		}
		else
		{
			o_stream << "Success: " << i_result.getT();
		}

		return o_stream;
	}
	friend std::string operator<<(const std::string& i_str, const result<T,Error>& i_result)
	{
		std::ostringstream res;

		if (i_result.hasError())
		{
			res << "Error: " << i_result.getError();
		}
		else
		{
			res << "Success " << i_result.getT();
		}

		return res.str();
	}

public:
	typedef T payload_t;
	typedef Error error_t;

	result(const T& i_payload);
	result(T&& i_payload);
	result(const Error& i_error);
	TEMPLATE(typename TT)
	REQUIRES(IS_SAME_CLASS(T,TT),IS_COPY_CONSTRUCTIBLE(TT))
	result(const result<TT,Error>& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_SAME_CLASS(T,TT),IS_MOVE_CONSTRUCTIBLE(TT))
	result(result<TT,Error>&& other);
	~result();
	TEMPLATE(typename TT)
	REQUIRES(IS_SAME_CLASS(T,TT),IS_COPY_ASSIGNABLE(TT))
	result& operator=(const result<TT,Error>& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_SAME_CLASS(T,TT),IS_MOVE_ASSIGNABLE(TT))
	result& operator=(result<TT,Error>&& other);
	Error error() const;
	T get() const;
	T extract() &&;
	explicit operator bool() const;
	result& dismiss();
	const result& dismiss() const;
	bool operator==(const result_success_t&) const;
	bool operator!=(const result_success_t&) const;
	bool operator==(const Error& i_error) const;
	bool operator!=(const Error& i_error) const;

private:
	variant<T,Error> m_nestedRes;

#if defined(DDK_DEBUG)
	mutable bool m_checked = false;
#endif
};


template<typename Result>
inline Result make_result()
{
	return  Result(success);
}
template<typename Result, typename ... Args>
inline Result make_result(Args&& ... i_args)
{
	return  typename Result::payload_t(std::forward<Args>(i_args) ...);
}

}

#include "ddk_result.inl"
