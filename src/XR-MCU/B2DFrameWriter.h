#pragma once

#include "IFrameWriter.h"
#include "bin2dash.hpp"

namespace mcu
{

class B2DFrameWriter : public IFrameWriter
{
public:
	B2DFrameWriter(const std::string& i_exchangeName, const std::string& i_publishUrl);
	~B2DFrameWriter();

private:
	send_result send(const EncodedFrame&) override;

	vrt_handle* m_b2dHandle;
};

}