#include "Position.hpp"
#include "Puzzle.hpp"

#include <iostream>

int main(int argc, char const *argv[])
{
	Puzzle puz(3);

	puz.at(1, 2) = 5;
	puz[1][2] = 4;
	puz.at({ 2, 1 }) = 3;
	std::cout << puz << std::endl;
	return 0;
}
