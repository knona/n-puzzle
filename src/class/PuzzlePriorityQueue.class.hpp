#ifndef PUZZLE_PRIORITY_QUEUE_HPP
#define PUZZLE_PRIORITY_QUEUE_HPP

#include "Puzzle.class.hpp"

#include <queue>
#include <unordered_set>
#include <vector>

class PuzzlePriorityQueue
{
	private:
	struct CompareFunction
	{
		bool operator()(const Puzzle &p1, const Puzzle &p2);
	};

	std::priority_queue<Puzzle, std::vector<Puzzle>, CompareFunction> _queue;
	std::unordered_set<size_t>                                        _umap;

	public:
	PuzzlePriorityQueue();
	PuzzlePriorityQueue(const PuzzlePriorityQueue &);
	PuzzlePriorityQueue &operator=(const PuzzlePriorityQueue &);
	PuzzlePriorityQueue(PuzzlePriorityQueue &&);
	PuzzlePriorityQueue &operator=(PuzzlePriorityQueue &&);
	~PuzzlePriorityQueue();

	int           size();
	bool          empty();
	void          push(const Puzzle &puzzle);
	const Puzzle &top() const;
	void          pop();
	bool          checkIsFound(const Puzzle &puzzle) const;
};

#endif // !PUZZLE_PRIORITY_QUEUE_HPP
