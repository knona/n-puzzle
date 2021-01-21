#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include "Array.class.hpp"
#include "Puzzle.class.hpp"

#include <vector>

class Heuristic
{
	private:
	static int              getHighestConflict(const Array<int> &array, int size);
	static bool             rowConflictsRemain(const Array<int> &array, int size);
	static std::vector<int> listConflictsRow(const Puzzle &puzzle, int row, int itj, int size);
	static std::vector<int> listConflictsCol(const Puzzle &puzzle, int col, int itj, int size);

	public:
	static int manhattan(const Puzzle &puzzle);
	static int hamming(const Puzzle &puzzle);
	static int linearConflicts(const Puzzle &puzzle);
};

#endif // !HEURISTIC_HPP
