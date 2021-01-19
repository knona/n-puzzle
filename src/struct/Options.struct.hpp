#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <optional>
#include <string>

struct Options
{
	bool                       parseOnly;
	std::optional<std::string> file;
	bool                       enableGui;
};

#endif // !OPTIONS_HPP
