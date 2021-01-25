#include "PathFinding.class.hpp"

#include "Heuristic.class.hpp"
#include "PuzzlePriorityQueue.class.hpp"

PathFinding::PathFinding(): _timeComplexity(0), _sizeComplexity(1)
{}

PathFinding::~PathFinding()
{}

std::list<Puzzle> PathFinding::getList(CameFromMap &cameFrom, const Puzzle &start, const Puzzle &goal) const
{
	std::list<Puzzle> solution;

	solution.push_front(goal);
	while (solution.front() != start)
		solution.push_front(Puzzle(cameFrom[solution.front().getData()]));

	return solution;
}

bool PathFinding::UniformCostCompareFunction::operator()(const Puzzle &p1, const Puzzle &p2)
{
	return p1.getG() > p2.getG();
};

std::list<Puzzle> PathFinding::uniformCost(Puzzle &start)
{
	PuzzlePriorityQueue<UniformCostCompareFunction> opened;
	ClosedSet                                       closed;
	CameFromMap                                     cameFrom;

	Puzzle goal = Puzzle::getGoal();
	start.setG(0);
	opened.push(start);

	while (!opened.empty())
	{
		Puzzle current = opened.top();

		this->_timeComplexity++;
		if (current == goal)
			return this->getList(cameFrom, start, goal);

		opened.pop();
		closed.insert(current.getData());
		for (Puzzle &child: current.getChildren())
		{
			bool foundInOpened = opened.checkIsFound(child);
			bool foundInClosed = closed.find(child.getData()) != closed.end();
			if (!foundInOpened && !foundInClosed)
			{
				child.setG(current.getG() + 1);
				cameFrom[child.getData()] = current.getData();
				opened.push(child);
			}
			else if (child.getG() > current.getG() + 1)
			{
				child.setG(current.getG() + 1);
				cameFrom[child.getData()] = current.getData();
				if (foundInClosed)
				{
					closed.erase(child.getData());
					opened.push(child);
				}
			}
		}
		if (opened.size() + closed.size() > this->_sizeComplexity)
			this->_sizeComplexity = opened.size() + closed.size();
	}
	return {};
}

bool PathFinding::GreedyCompareFunction::operator()(const Puzzle &p1, const Puzzle &p2)
{
	return p1.getH() > p2.getH();
};

std::list<Puzzle> PathFinding::greedy(Puzzle &start)
{
	PuzzlePriorityQueue<GreedyCompareFunction> opened;
	ClosedSet                                  closed;
	CameFromMap                                cameFrom;

	Puzzle goal = Puzzle::getGoal();
	start.setH();
	opened.push(start);

	while (!opened.empty())
	{
		Puzzle current = opened.top();

		this->_timeComplexity++;
		if (current == goal)
			return this->getList(cameFrom, start, goal);

		opened.pop();
		closed.insert(current.getData());
		for (Puzzle &child: current.getChildren())
		{
			bool foundInOpened = opened.checkIsFound(child);
			bool foundInClosed = closed.find(child.getData()) != closed.end();
			if (!foundInOpened && !foundInClosed)
			{
				cameFrom[child.getData()] = current.getData();
				child.setH();
				opened.push(child);
			}
		}
		if (opened.size() + closed.size() > this->_sizeComplexity)
			this->_sizeComplexity = opened.size() + closed.size();
	}
	return {};
}

bool PathFinding::AStarCompareFunction::operator()(const Puzzle &p1, const Puzzle &p2)
{
	return p1.getG() + p1.getH() > p2.getG() + p2.getH();
};

std::list<Puzzle> PathFinding::aStar(Puzzle &start)
{
	PuzzlePriorityQueue<AStarCompareFunction> opened;
	ClosedSet                                 closed;
	CameFromMap                               cameFrom;

	Puzzle goal = Puzzle::getGoal();
	start.setG(0);
	start.setH();
	opened.push(start);

	while (!opened.empty())
	{
		Puzzle current = opened.top();

		this->_timeComplexity++;
		if (current == goal)
			return this->getList(cameFrom, start, goal);

		opened.pop();
		closed.insert(current.getData());
		for (Puzzle &child: current.getChildren())
		{
			bool foundInOpened = opened.checkIsFound(child);
			bool foundInClosed = closed.find(child.getData()) != closed.end();
			if (!foundInOpened && !foundInClosed)
			{
				child.setG(current.getG() + 1);
				cameFrom[child.getData()] = current.getData();
				child.setH();
				opened.push(child);
			}
			else if (child.getG() > current.getG() + 1)
			{
				child.setG(current.getG() + 1);
				cameFrom[child.getData()] = current.getData();
				if (foundInClosed)
				{
					closed.erase(child.getData());
					child.setH();
					opened.push(child);
				}
			}
		}
		if (opened.size() + closed.size() > this->_sizeComplexity)
			this->_sizeComplexity = opened.size() + closed.size();
	}
	return {};
}

void PathFinding::printDuration(std::chrono::duration<double, std::milli> fpMs) const
{
	auto m = std::chrono::duration_cast<std::chrono::minutes>(fpMs);
	auto s = std::chrono::duration_cast<std::chrono::seconds>(fpMs -= m);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(fpMs -= s);
	auto us = std::chrono::duration_cast<std::chrono::microseconds>(fpMs -= ms);
	std::cout << "Computed time to solution: ";
	if (m.count())
		std::cout << "\033[0;36m" << m.count() << "\033[0m minutes, ";
	if (s.count())
		std::cout << "\033[0;36m" << s.count() << "\033[0m seconds, ";
	if (ms.count())
		std::cout << "\033[0;36m" << ms.count() << "\033[0m milliseconds, ";
	std::cout << "\033[0;36m" << us.count() << "\033[0m microseconds";
	std::cout << std::endl;
}

std::list<Puzzle> PathFinding::resolve(Puzzle &start, Algorithm algorithm)
{
	std::list<Puzzle> list;

	std::cout << "\033[0;33mProcessing...\033[0m" << std::endl;
	auto t1 = std::chrono::high_resolution_clock::now();
	if (algorithm == Algorithm::aStar)
		list = this->aStar(start);
	else if (algorithm == Algorithm::greedy)
		list = this->greedy(start);
	else if (algorithm == Algorithm::uniformCost)
		list = this->uniformCost(start);
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << "\033[0;32mFinished\033[0m" << std::endl;
	this->printDuration(t2 - t1);
	std::cout << "Number of moves: \033[0;36m" << list.size() - 1 << "\033[0m" << std::endl;
	std::cout << "Time complexity: \033[0;36m" << this->_timeComplexity << "\033[0m" << std::endl;
	std::cout << "Size complexity: \033[0;36m" << this->_sizeComplexity << "\033[0m" << std::endl;
	std::cout << std::endl;
	return list;
}
