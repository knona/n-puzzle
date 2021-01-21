#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <optional>
#include <string>
#include "Heuristic.enum.hpp"

struct Options
{
	bool                       parseOnly;
	std::optional<std::string> file;
	bool                       enableGui;
	Heuristic 				   heuristic;
};

#endif // !OPTIONS_HPP
