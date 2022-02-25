#pragma once

#include "FrameDefs.h"

namespace mcu
{

struct reader_id_t;
typedef ddk::Id<std::string,reader_id_t> ReaderId;

enum class FrameReaderType
{
	Sub,
	Socketio
};


struct sub_reader_init_context
{
public:
	sub_reader_init_context(const std::string& i_url, ReaderId i_id);

	const std::string& get_url() const;
	ReaderId get_id() const;

private:
	const std::string m_url;
	const ReaderId m_id;
};

}