#pragma once

#include <vector>
#include "ddk_connection.h"

namespace mcu
{

class MCUMeasurementMgr
{
public:
	MCUMeasurementMgr() = default;

	void Init();
	void Deinit();

private:
	std::vector<ddk::connection> m_measuremntConn;
};

}