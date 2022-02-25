#pragma once

#include <exception>
#include <string>

namespace ddk
{

struct call_function_exception : public std::exception
{
public:
    call_function_exception(const std::string& i_reason);

    std::string reason() const;

private:
    const std::string m_reason;
};

}
