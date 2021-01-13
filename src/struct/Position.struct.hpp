#ifndef POSITION_HPP
#define POSITION_HPP

#include <iostream>

typedef unsigned int uint;

struct Position
{
	uint y;
	uint x;
};

std::ostream &operator<<(std::ostream &os, const Position &pos);

#endif // !POSITION_HPP
