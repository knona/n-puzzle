#ifndef POSITION_HPP
#define POSITION_HPP

#include <iostream>

struct Position
{
	int y;
	int x;
};

std::ostream &operator<<(std::ostream &os, const Position &pos);

#endif // !POSITION_HPP
