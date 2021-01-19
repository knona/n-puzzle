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

//////////////////////////////////////////////////////////////////

// Aim of the function :
// Returns the amount of linear conflicts in a given row, for a given tile, according to the conflict definition:
// Two tiles tj and tk are in a linear conflict if tj and tk are in the same line, the goal positions of tj and tk are both in that line,
// tj is to the right of tk, and goal position of tj is to the left of the goal position of tk
int	nb_conflicts_row(const Puzzle &puzzle, int row, int i_tj, int size)
{
    int conflicts = 0;
    int tj = puzzle.getAt(row, i_tj);

	Position &goal_pos_tj = Heuristic::goalMap[tj];
	// If the current tile is positionned, in the goal puzzle, inside the same row
	if (goal_pos_tj.y == row)
	{
		// Loop on all tiles in current row of the puzzle
		for (int i_tk = 0; i_tk < size; i_tk++)
		{
			int tk = puzzle.getAt(row, i_tk);
			Position &goal_pos_tk = Heuristic::goalMap[tk];
			// if tj and tk are different, not equal to 0 (empty tile) and tk is at same row as tj in goal puzzle
			if (tk != tj && tk != 0 && tj != 0 && goal_pos_tk.y == row)
			{
				// if tj is on the right side of tk and tj's goal pos on the left of tk's goal pos OR contrary
				if ((i_tj > i_tk && goal_pos_tj.x < goal_pos_tk.x)
					|| (i_tj < i_tk && goal_pos_tj.x > goal_pos_tk.x))
				{
					conflicts += 1;
				}
			}
		}
	}
    return conflicts;
}

// Aim of the function:
// Returns the position of the highest element in the Array
int get_highest_conflict(Array<int> array, int size)
{
	int i_highest = 0;
	int highest = 0;
    for (int i = 0; i < size; i++)
	{
		if (array[i] > highest)
		{
			highest = array[i];
			i_highest = i;
		}
	}
	return i_highest;
}

// Aim of the function:
// Returns true if array still contains elements > 0
bool row_conflicts_remain(Array<int> array, int size)
{
    for (int i = 0; i < size; i++)
	{
		if (array[i] > 0)
		{
			return true;
		}
	}
	return false;
}

// Aim of the function:
// Returns the position of the conflicting tiles, for a given row and a given tile
std::vector<int> list_conflicts_row(const Puzzle &puzzle, int row, int i_tj, int size)
{
    std::vector<int> vec;
    int tj = puzzle.getAt(row, i_tj);

	Position &goal_pos_tj = Heuristic::goalMap[tj];
	// If the current tile is positionned, in the goal puzzle, inside the same row
	if (goal_pos_tj.y == row)
	{
		// Loop on all tiles in current row of the puzzle
		for (int i_tk = 0; i_tk < size; i_tk++)
		{
			int tk = puzzle.getAt(row, i_tk);
			Position &goal_pos_tk = Heuristic::goalMap[tk];
			// if tj and tk are different, not equal to 0 (empty tile) and tk is at same row as tj in goal puzzle
			if (tk != tj && tk != 0 && tj != 0 && goal_pos_tk.y == row)
			{
				// if tj is on the right side of tk and tj's goal pos on the left of tk's goal pos OR contrary
				if ((i_tj > i_tk && goal_pos_tj.x < goal_pos_tk.x)
					|| (i_tj < i_tk && goal_pos_tj.x > goal_pos_tk.x))
				{
					vec.push_back(i_tk);
				}
			}
		}
	}
    return vec;
}

// Aim of the function :
// Returns the amount of linear conflicts in a given row, for a given tile, according to the conflict definition:
// Two tiles tj and tk are in a linear conflict if tj and tk are in the same line, the goal positions of tj and tk are both in that line,
// tj is to the right of tk, and goal position of tj is to the left of the goal position of tk
int	nb_conflicts_col(const Puzzle &puzzle, int col, int i_tj, int size)
{
    int conflicts = 0;
    int tj = puzzle.getAt(i_tj, col);

	Position &goal_pos_tj = Heuristic::goalMap[tj];
	// If the current tile is positionned, in the goal puzzle, inside the same row
	if (goal_pos_tj.x == col)
	{
		// Loop on all tiles in current row of the puzzle
		for (int i_tk = 0; i_tk < size; i_tk++)
		{
			int tk = puzzle.getAt(i_tk, col);
			Position &goal_pos_tk = Heuristic::goalMap[tk];
			// if tj and tk are different, not equal to 0 (empty tile) and tk is at same row as tj in goal puzzle
			if (tk != tj && tk != 0 && tj != 0 && goal_pos_tk.x == col)
			{
				// if tj is on the right side of tk and tj's goal pos on the left of tk's goal pos OR contrary
				if ((i_tj > i_tk && goal_pos_tj.y < goal_pos_tk.y)
					|| (i_tj < i_tk && goal_pos_tj.y > goal_pos_tk.y))
				{
					conflicts += 1;
				}
			}
		}
	}
    return conflicts;
}

// Transposed function :
// We now work on columns, and check for the x instead of the y
std::vector<int> list_conflicts_col(const Puzzle &puzzle, int col, int i_tj, int size)
{
	std::vector<int> vec;
    int tj = puzzle.getAt(i_tj, col);

	Position &goal_pos_tj = Heuristic::goalMap[tj];
	// If the current tile is positionned, in the goal puzzle, inside the same row
	if (goal_pos_tj.x == col)
	{
		// Loop on all tiles in current row of the puzzle
		for (int i_tk = 0; i_tk < size; i_tk++)
		{
			int tk = puzzle.getAt(i_tk, col);
			Position &goal_pos_tk = Heuristic::goalMap[tk];
			// if tj and tk are different, not equal to 0 (empty tile) and tk is at same row as tj in goal puzzle
			if (tk != tj && tk != 0 && tj != 0 && goal_pos_tk.x == col)
			{
				// if tj is on the right side of tk and tj's goal pos on the left of tk's goal pos OR contrary
				if ((i_tj > i_tk && goal_pos_tj.y < goal_pos_tk.y)
					|| (i_tj < i_tk && goal_pos_tj.y > goal_pos_tk.y))
				{
					vec.push_back(i_tk);
				}
			}
		}
	}
    return vec;
}

int Heuristic::linear_conflicts(const Puzzle &puzzle)
{
	int size = Puzzle::getSize();
	int h = Heuristic::manhattan(puzzle);
	Array<int> crn(size, 0);// conflicts row number
	Array<int> ccn(size, 0);// conflicts column number
	int nb_lc = 0;// score linear conflicts


	
	// Loop on rows
	for (int row = 0; row < size; row++)
	{
		// Loop on tiles, called tilesj (tj), and
		// store nb of conflict for each tile on this row
		for (int i_tj = 0; i_tj < size; i_tj++)
		{
			crn[i_tj] = nb_conflicts_row(puzzle, row, i_tj, size);
		}
		while (row_conflicts_remain(crn, size))
		{
			// Pop the tile with the biggest amount of conflict
			int i_tk = get_highest_conflict(crn, size);
            // Set it at 0
			crn[i_tk] = 0;
			// Retrieve list of position the tile is in conflict with
			std::vector<int> vec = list_conflicts_row(puzzle, row, i_tk, size);
			// Loop on them and remove one when processed
			for (int i_tj = 0; i_tj < static_cast<int>(vec.size()); i_tj++)
			{
				crn[vec[i_tj]] -= 1; // Remove one to related conflict
			}
			nb_lc += 1; // Each removed conflicted equals one conflict
		}
	}
	// Transpose to columns
	// Need to implement same, but with columns
	for (int col = 0; col < size; col++)
	{
		// Loop on tiles, called tilesj (tj), and
		// store nb of conflict for each tile on this row
		for (int i_tj = 0; i_tj < size; i_tj++)
		{
			ccn[i_tj] = nb_conflicts_col(puzzle, col, i_tj, size);
		}
		while (row_conflicts_remain(ccn, size))
		{
			// Pop the tile with the biggest amount of conflict
			int i_tk = get_highest_conflict(ccn, size);
            // Set it at 0
			ccn[i_tk] = 0;
			// Retrieve list of position the tile is in conflict with
			std::vector<int> vec = list_conflicts_col(puzzle, col, i_tk, size);
			// Loop on them and remove one when processed
			for (int i_tj = 0; i_tj < static_cast<int>(vec.size()); i_tj++)
			{
				ccn[vec[i_tj]] -= 1; // Remove one to related conflict
			}
			nb_lc += 1; // Each removed conflicted equals one conflict
		}
	}
	return h + 2 * nb_lc;
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
