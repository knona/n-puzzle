#include "PuzzlePriorityQueue.class.hpp"

template<typename CompareFunction>
PuzzlePriorityQueue<CompareFunction>::PuzzlePriorityQueue()
{}

template<typename CompareFunction>
PuzzlePriorityQueue<CompareFunction>::PuzzlePriorityQueue(const PuzzlePriorityQueue<CompareFunction> &container)
{
	*this = container;
}

template<typename CompareFunction>
PuzzlePriorityQueue<CompareFunction> &
PuzzlePriorityQueue<CompareFunction>::operator=(const PuzzlePriorityQueue<CompareFunction> &container)
{
	if (this != &container)
	{
		this->_queue = container._queue;
		this->_umap = container._umap;
	}
	return *this;
}

template<typename CompareFunction>
PuzzlePriorityQueue<CompareFunction>::PuzzlePriorityQueue(PuzzlePriorityQueue<CompareFunction> &&container)
{
	*this = std::move(container);
}

template<typename CompareFunction>
PuzzlePriorityQueue<CompareFunction> &
PuzzlePriorityQueue<CompareFunction>::operator=(PuzzlePriorityQueue<CompareFunction> &&container)
{
	if (this != &container)
	{
		this->_queue = std::move(container._queue);
		this->_umap = std::move(container._umap);
	}
	return *this;
}

template<typename CompareFunction>
PuzzlePriorityQueue<CompareFunction>::~PuzzlePriorityQueue()
{}

template<typename CompareFunction>
int PuzzlePriorityQueue<CompareFunction>::size()
{
	return this->_queue.size();
}

template<typename CompareFunction>
bool PuzzlePriorityQueue<CompareFunction>::empty()
{
	return this->_queue.empty();
}

template<typename CompareFunction>
void PuzzlePriorityQueue<CompareFunction>::push(const Puzzle &puzzle)
{
	this->_queue.push(puzzle);
	this->_umap.insert(puzzle.getData());
}

template<typename CompareFunction>
const Puzzle &PuzzlePriorityQueue<CompareFunction>::top() const
{
	return this->_queue.top();
}

template<typename CompareFunction>
void PuzzlePriorityQueue<CompareFunction>::pop()
{
	this->_umap.erase(this->top().getData());
	this->_queue.pop();
}

template<typename CompareFunction>
bool PuzzlePriorityQueue<CompareFunction>::checkIsFound(const Puzzle &puzzle) const
{
	return this->_umap.find(puzzle.getData()) != this->_umap.end();
}
