#pragma once

#include <exception>
#include <string>

namespace ddk
{

class bad_memory_exception : public std::exception
{
};

class bad_alignment_exception : public bad_memory_exception
{
public:
	bad_alignment_exception(const std::string& i_reason, void* i_ptr);

	std::string reason() const;
	void* get_alloc() const;

private:
	const std::string m_reason;
	void* m_ptr;
};

class bad_allocation_exception : public bad_memory_exception
{
public:
	bad_allocation_exception(const std::string& i_reason);

	std::string reason() const;

private:
	const std::string m_reason;
};

class bad_deallocation_exception : public bad_memory_exception
{
public:
	bad_deallocation_exception(const std::string& i_reason);

	std::string reason() const;

private:
	const std::string m_reason;
};

}
