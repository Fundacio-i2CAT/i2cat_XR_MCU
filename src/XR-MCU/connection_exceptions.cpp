#include "connection_exceptions.h"

namespace mcu
{

connection_exception::connection_exception(const std::string& i_reason, int i_errorCode)
: m_reason(i_reason)
, m_code(i_errorCode)
{
}
const char* connection_exception::what() const
{
	return m_reason.c_str();
}
int connection_exception::get_code() const
{
	return m_code;
}

}