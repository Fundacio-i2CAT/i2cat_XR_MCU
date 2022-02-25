#pragma once

#include <exception>
#include <string>

namespace ddk
{

class bad_access_exception : public std::exception
{
public:
    bad_access_exception(const std::string& i_reason);

    std::string reason() const;

private:
    const std::string m_reason;
};

}
