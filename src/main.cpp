#include "Position.hpp"
#include "Puzzle.hpp"

#include <iostream>

int main(int argc, char const *argv[])
{
	if (argc > 2)
	{
		std::cerr << "Usage: " << argv[0] << " [puzzle file]" << std::endl;
		return 1;
	}

	Puzzle puzzle;
	try
	{
		puzzle = parser(argv[1]);
	}
	catch (const std::exception &e)
	{
		std::cerr << "\033[0;31mError:\033[0m" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	std::cout << puzzle << std::endl;
	return 0;
}
