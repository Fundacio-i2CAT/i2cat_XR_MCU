#pragma once

#include <exception>

namespace mcu
{

struct FusionMergeException : public std::exception
{
};

struct FusionCreationException : public std::exception
{
};

struct FusionEncoderException : public std::exception
{
};

struct FusionCollectorException : public std::exception
{
};

struct FusionSenderException : public std::exception
{
};

}