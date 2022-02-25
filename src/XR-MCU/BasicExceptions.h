#pragma once

#include <exception>

namespace mcu
{

class BadAccessException : public std::exception
{
};

}