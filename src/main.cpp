#include "Position.hpp"
#include "Puzzle.hpp"

#include <iostream>

int main(int argc, char const *argv[])
{
	if (argc > 2)
	{
		std::cerr << "Usage: " << argv[0] << " [file]";
		return 1;
	}

	Puzzle puz;
	try
	{
		puz = parser(argv[1]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	std::cout << puz << std::endl;
	return 0;
}
