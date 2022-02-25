#pragma once

#include <exception>
#include <string>

namespace mcu
{

struct bad_config : std::exception
{
public:
	bad_config(const std::string& i_reason);

	const char* what() const override;

private:
	const std::string m_reason;
};

}