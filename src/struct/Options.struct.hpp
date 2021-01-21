#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "PathFinding.class.hpp"

#include <optional>
#include <string>

struct Options
{
	bool                       parseOnly;
	std::optional<std::string> file;
	bool                       enableGui;
	PathFinding::Algorithm     algorithm;
};

#endif // !OPTIONS_HPP
