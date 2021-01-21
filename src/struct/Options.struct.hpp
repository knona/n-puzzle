#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "Heuristic.class.hpp"

#include <optional>
#include <string>

struct Options
{
	bool                       parseOnly;
	std::optional<std::string> file;
	bool                       enableGui;
	Heuristic::Value           heuristic;
};

#endif // !OPTIONS_HPP
