#pragma once

#include <exception>
#include <string>

namespace mcu
{

struct connection_exception : std::exception
{
public:
	connection_exception(const std::string& i_reason, int i_errorCode);

	const char* what() const override;
	int get_code() const;

private:
	const std::string m_reason;
	int m_code;
};

}