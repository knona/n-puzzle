#include "CommandLine.class.hpp"
#include "Gui.class.hpp"
#include "Options.struct.hpp"
#include "Parser.class.hpp"
#include "PathFinding.class.hpp"
#include "Puzzle.class.hpp"

#include <exception>
#include <iostream>
#include <list>
#include <vector>

void displaySolution(const Options &options, const std::list<Puzzle> &list)
{
	if (options.enableGui)
	{
		Gui gui;
		gui.init();
		gui.render(std::vector<Puzzle>(list.begin(), list.end()));
	}
	else if (!options.quiet)
	{
		std::cout << "Solution: " << std::endl;
		for (const Puzzle &puzzle: list)
			std::cout << puzzle << std::endl;
	}
}

int main(int argc, char const *argv[])
{
	Options     options;
	PathFinding pathFinding;

	try
	{
		if (!CommandLine::getOptions(argc, argv, options))
			return 1;

		Parser parser(options.file);
		parser.parse();
		if (options.parseOnly)
		{
			parser.getPuzzle().print(std::cout, true);
			return 0;
		}
		Puzzle start = parser.getPuzzle();
		if (!start.checkIsSolvable())
			throw std::logic_error("The puzzle is unsolvable");
		std::list<Puzzle> list = pathFinding.resolve(start, options.algorithm);
		displaySolution(options, list);
	}
	catch (const std::exception &e)
	{
		std::cerr << "\033[0;31mError:\033[0m" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
