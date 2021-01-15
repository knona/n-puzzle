#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include "Array.class.hpp"
#include "Puzzle.class.hpp"

class Heuristic
{
	private:
	static Puzzle          goal;
	static Array<Position> goalMap;

	public:
	static int  manhattan(const Puzzle &puzzle);
	static int  hamming(const Puzzle &puzzle);
	static void init(int size);
};

#endif // !HEURISTIC_HPP
