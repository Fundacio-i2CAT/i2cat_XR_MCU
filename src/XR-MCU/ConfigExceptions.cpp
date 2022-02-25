#include "ConfigExceptions.h"

namespace mcu
{

bad_config::bad_config(const std::string& i_reason)
: m_reason(i_reason)
{
}
const char* bad_config::what() const
{
	return m_reason.c_str();
}

}