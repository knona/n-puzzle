#include "Parser.class.hpp"
#include "Position.struct.hpp"
#include "Puzzle.class.hpp"

#include <exception>
#include <iostream>

typedef unsigned int uint;

int main(int argc, char const *argv[])
{
	if (argc > 2)
	{
		std::cerr << "Usage: " << argv[0] << " [puzzle file]" << std::endl;
		return 1;
	}

	Parser parser(argv[1]);
	try
	{
		parser.parse();
	}
	catch (const std::exception &e)
	{
		std::cerr << "\033[0;31mError:\033[0m" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	std::cout << parser.getPuzzle() << std::endl;
	return 0;
}
