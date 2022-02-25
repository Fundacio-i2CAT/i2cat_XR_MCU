#include "FrameReaderDefs.h"

namespace mcu
{

sub_reader_init_context::sub_reader_init_context(const std::string& i_url, ReaderId i_id)
: m_url(i_url)
, m_id(i_id)
{
}
const std::string& sub_reader_init_context::get_url() const
{
	return m_url;
}
ReaderId sub_reader_init_context::get_id() const
{
	return m_id;
}

}