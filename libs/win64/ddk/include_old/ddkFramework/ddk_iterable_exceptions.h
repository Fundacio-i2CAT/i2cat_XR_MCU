#pragma once

#include <exception>
#include <string>

namespace ddk
{

class iterable_filtered_out_exception : public std::exception
{
public:
	iterable_filtered_out_exception() = default;
};

class iterable_operation_forbidden_exception : public std::exception
{
public:
	iterable_operation_forbidden_exception(const char* i_reason);

    const char* what() const noexcept;

private:
    std::string m_reason;
};

class iteration_exception : public std::exception
{
public:
	iteration_exception(const char* i_reason);

	const char* what() const noexcept;

private:
	std::string m_reason;
};

}
