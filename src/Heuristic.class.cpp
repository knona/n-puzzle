#include "Heuristic.class.hpp"

Puzzle          Heuristic::goal;
Array<Position> Heuristic::goalMap;

void Heuristic::init()
{
	int size = Puzzle::getSize();
	Heuristic::goal = Puzzle::getGoal();
	Heuristic::goalMap = Array<Position>(size * size);
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
			Heuristic::goalMap[Heuristic::goal.getAt(y, x)] = { y, x };
	}
}

int Heuristic::manhattan(const Puzzle &puzzle)
{
	int size = Puzzle::getSize();
	int total = 0;

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			Position &goalPos = Heuristic::goalMap[puzzle.getAt(y, x)];
			total += std::abs(goalPos.y - y) + abs(goalPos.x - x);
		}
	}
	return total;
}

int Heuristic::hamming(const Puzzle &puzzle)
{
	int size = Puzzle::getSize();
	int total = 0;

	for (int i = 0; i < size * size; i++)
	{
		if (puzzle[i] != Heuristic::goal[i])
			total += 1;
	}

	return total;
}
