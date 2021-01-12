#include "Exception.class.hpp"

const char *Exception::GameOver::what() const throw()
{
	return "\033[0;31mGAME OVER\033[0m";
}

Exception::Parser::Parser(const std::string line, uint lineNb, uint col, const std::string &detail)
{
	std::stringstream buffer;
	buffer << detail << " \033[0;36m[" << lineNb << ":" << col << "]\033[0m" << std::endl;
	buffer << line << std::endl;
	for (uint i = 0; i < col; i++)
		buffer << " ";
	buffer << "\033[0;31m^\033[0m";
	Exception::error = buffer.str();
}

const char *Exception::Parser::Parser::what() const throw()
{
	return Exception::error.c_str();
}

Exception::ParserLight::ParserLight(const std::string &detail, bool addPosition)
{
	this->_addPosition = addPosition;
	Exception::error = detail;
}

bool Exception::ParserLight::getAddPosition() const
{
	return this->_addPosition;
}

const char *Exception::ParserLight::ParserLight::what() const throw()
{
	return Exception::error.c_str();
}

std::string Exception::error;
