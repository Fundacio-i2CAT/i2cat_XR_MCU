#pragma once

#include "ICpuResource.h"

namespace mcu
{

class CpuResource : public ICpuResource
{
public:

private:
	void set_affinity(const cpu_set_t& i_cpu_set) override;

};

}