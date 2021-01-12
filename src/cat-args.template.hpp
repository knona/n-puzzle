#ifndef CAT_ARGS_TPP
#define CAT_ARGS_TPP

#include <sstream>

template<typename... Args>
std::string catArgs(const Args &...args)
{
	std::stringstream sstream;

	(sstream << ... << args);
	return sstream.str();
}

#endif // !CAT_ARGS_TPP
