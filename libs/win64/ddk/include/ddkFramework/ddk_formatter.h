#pragma once

#include <string>
#include <set>
#include <vector>
#include <utility>
#include <sstream>
#include "ddk_macros.h"

namespace ddk
{

template<typename T>
struct formatter
{
	static_assert(sizeof(T) == 0, "Go and add an overload for this type");
};

template<>
struct formatter<bool>
{
static bool format(const std::string& i_str);
};
template<>
struct formatter<char>
{
static char format(const std::string& i_str);
};
template<>
struct formatter<unsigned char>
{
static unsigned char format(const std::string& i_str);
};
template<>
struct formatter<short>
{
static short format(const std::string& i_str);
};
template<>
struct formatter<unsigned short>
{
static unsigned short format(const std::string& i_str);
};
template<>
struct formatter<int>
{
static int format(const std::string& i_str);
};
template<>
struct formatter<unsigned int>
{
static unsigned int format(const std::string& i_str);
};
template<>
struct formatter<long>
{
static long format(const std::string& i_str);
};
template<>
struct formatter<unsigned long>
{
static unsigned long format(const std::string& i_str);
};
template<>
struct formatter<long long>
{
static long long format(const std::string& i_str);
};
template<>
struct formatter<unsigned long long>
{
static unsigned long long format(const std::string& i_str);
};
template<>
struct formatter<float>
{
static float format(const std::string& i_str);
};
template<>
struct formatter<double>
{
static double format(const std::string& i_str);
};
template<>
struct formatter<std::string>
{
static std::string format(const std::string& i_str);
template<typename T>
static std::string format(const T& i_val)
{
	std::ostringstream convert;

	convert << i_val;

	return convert.str();
}
template<typename T>
static std::string format(const std::set<T>& i_val)
{
	std::ostringstream convert;
	bool firstItem = true;

	typename std::set<T>::const_iterator itVal = i_val.begin();
	for(;itVal!=i_val.end();++itVal)
	{
		if(firstItem == false)
		{
			convert << ",";
		}

		convert << format(*itVal);
		firstItem = false;
	}

	return convert.str();
}
template<typename T>
static std::string format(const std::vector<T>& i_val)
{
	std::ostringstream convert;
	bool firstItem = true;

	typename std::vector<T>::const_iterator itVal = i_val.begin();
	for(;itVal!=i_val.end();++itVal)
	{
		if(firstItem == false)
		{
			convert << ",";
		}

		convert << format(*itVal);
		firstItem = false;
	}

	return convert.str();
}
};
template<typename First, typename Second>
struct formatter<std::pair<First,Second>>
{
static std::pair<First,Second> format(const std::string& i_str)
{
	std::pair<First,Second> res;

	if (i_str.empty() == false)
	{
		size_t separatorPos = i_str.find(",");
		if (separatorPos != std::string::npos && separatorPos < i_str.size() - 1)
		{
			res.first = formatter<First>::format(i_str.substr(0,separatorPos));
			res.second = formatter<Second>::format(i_str.substr(separatorPos+1));
		}
		else
		{
			DDK_FAIL("Expected separator not present");
		}
	}

	return res;
}
};

template<typename T>
struct formatter<std::vector<T>>
{
static std::vector<T> format(const std::string& i_str)
{
	std::vector<T> res;

	if (i_str.empty() == false)
	{
		size_t preTokenPos = 0;
		size_t separatorPos = i_str.find(",");

		while(separatorPos != std::string::npos)
		{
			res.push_back(formatter<T>::format(i_str.substr(preTokenPos,separatorPos - preTokenPos)));

			preTokenPos = separatorPos + 1;
			separatorPos = i_str.find(",",preTokenPos);
		}
	}

	return res;
}
};

template<typename T>
struct formatter<std::set<T>>
{
static std::set<T> format(const std::string& i_str)
{
	std::set<T> res;

	if (i_str.empty() == false)
	{
		size_t preTokenPos = 0;
		size_t separatorPos = i_str.find(",");

		if (separatorPos != std::string::npos)
		{
			do
			{
				res.insert(formatter<T>::format(i_str.substr(preTokenPos,separatorPos)));

				preTokenPos = separatorPos + 1;
				separatorPos = i_str.find_first_of(",",preTokenPos);
			}
			while(separatorPos != std::string::npos);
		}
		else
		{
			res.insert(i_str);
		}
	}

	return res;
}
};

}
