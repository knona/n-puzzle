#ifndef CAT_ARGS_HPP
#define CAT_ARGS_HPP

#include <sstream>

template<typename... Args>
std::string catArgs(const Args &...args)
{
	std::stringstream sstream;

	(sstream << ... << args);
	return sstream.str();
}

#endif // !CAT_ARGS_HPP
