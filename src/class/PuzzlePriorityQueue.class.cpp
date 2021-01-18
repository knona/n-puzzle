#include "PuzzlePriorityQueue.class.hpp"

bool PuzzlePriorityQueue::CompareFunction::operator()(const Puzzle &p1, const Puzzle &p2)
{
	return (p1.getF() > p2.getF());
};

PuzzlePriorityQueue::PuzzlePriorityQueue()
{}

PuzzlePriorityQueue::~PuzzlePriorityQueue()
{}

int PuzzlePriorityQueue::size()
{
	return _queue.size();
}

bool PuzzlePriorityQueue::empty()
{
	return _queue.empty();
}

void PuzzlePriorityQueue::push(const Puzzle &puzzle)
{
	_queue.push(puzzle);
	_umap.insert(puzzle.getData());
}

const Puzzle &PuzzlePriorityQueue::top() const
{
	return _queue.top();
}

void PuzzlePriorityQueue::pop()
{
	_umap.erase(this->top().getData());
	_queue.pop();
}

bool PuzzlePriorityQueue::checkIsFound(const Puzzle &puzzle) const
{
	return this->_umap.find(puzzle.getData()) != this->_umap.end();
}
