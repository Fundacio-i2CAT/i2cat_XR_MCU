#pragma once

#include <cstdint>
#include <string>
#include <array>

#define MAX_PACKET 4096

namespace mcu
{

struct NetMessage
{
	std::string id;
	uint8_t type = 0;
	float position[3] = {0,0,0};
	float rotation = 0;
	int fov = 0;
	std::string url;
};

}