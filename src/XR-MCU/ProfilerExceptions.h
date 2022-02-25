#pragma once

#include <exception>

namespace pfr
{

struct BadMeasureAccess : public std::exception
{
};

struct NonExistentProfileMdule : public std::exception
{
};

struct NonExistentProfileMeasure : public std::exception
{
};

}