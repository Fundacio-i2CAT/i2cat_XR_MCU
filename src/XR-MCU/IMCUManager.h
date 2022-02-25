#pragma once

namespace mcu
{

class IMCUManager
{
public:
	~IMCUManager() = default;

	virtual void close() = 0;
};

}