#pragma once

#include <string>

namespace ddk
{
namespace detail
{

class Assert
{
public:
	enum Response
	{
		Break,
		Continue
	};

	Assert(const char* i_file, size_t i_fileLine, const std::string& i_title, const std::string& i_message);
	Assert(const char* i_expression, const char* i_file, size_t i_fileLine, const std::string& i_title, const std::string& i_message);
	std::string what() const;
	std::string getExpression() const;
	std::string getFile() const;
	size_t getLine() const;
	std::string getMessage();
	Response raise() const;

private:
    std::string m_expression;
    std::string m_file;
    size_t m_line;
	std::string m_title;
    std::string m_message;
	std::string m_output;
};

}
}