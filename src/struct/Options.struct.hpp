#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "PathFinding.class.hpp"

#include <optional>
#include <string>

struct Options
{
	bool                       parseOnly;
	bool                       enableGui;
	bool                       quiet;
	std::optional<std::string> file;
	PathFinding::Algorithm     algorithm;
};

#endif // !OPTIONS_HPP
