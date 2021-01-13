#include "Position.struct.hpp"

std::ostream &operator<<(std::ostream &os, const Position &pos)
{
	os << "y: " << pos.y << ", x: " << pos.x;
	return os;
}
