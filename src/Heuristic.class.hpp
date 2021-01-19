#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include "Array.class.hpp"
#include "Puzzle.class.hpp"
#include <vector>

class Heuristic
{
	private:
	static Puzzle          goal;

	public:
	static Array<Position> goalMap;
	static int  manhattan(const Puzzle &puzzle);
	static int  hamming(const Puzzle &puzzle);
	static int	linear_conflicts(const Puzzle &puzzle);
	static void init();
};

#endif // !HEURISTIC_HPP
