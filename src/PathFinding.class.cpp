#include "PathFinding.class.hpp"

#include "Heuristic.class.hpp"
#include "PuzzlePriorityQueue.class.hpp"

PathFinding::PathFinding()
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

std::list<Puzzle> PathFinding::uniformCost(Puzzle &start) const
{
	PuzzlePriorityQueue<UniformCostCompareFunction> opened;
	ClosedSet                                       closed;
	CameFromMap                                     cameFrom;

	Heuristic::init();
	Puzzle goal = Puzzle::getGoal();

	start.setG(0);
	opened.push(start);

	while (!opened.empty())
	{
		Puzzle current = opened.top();

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
	}
	return {};
}

bool PathFinding::GreedyCompareFunction::operator()(const Puzzle &p1, const Puzzle &p2)
{
	return p1.getH() > p2.getH();
};

std::list<Puzzle> PathFinding::greedy(Puzzle &start) const
{
	PuzzlePriorityQueue<GreedyCompareFunction> opened;
	ClosedSet                                  closed;
	CameFromMap                                cameFrom;

	Heuristic::init();
	Puzzle goal = Puzzle::getGoal();

	start.setH();
	opened.push(start);

	while (!opened.empty())
	{
		Puzzle current = opened.top();

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
	}
	return {};
}

bool PathFinding::AStarCompareFunction::operator()(const Puzzle &p1, const Puzzle &p2)
{
	return p1.getG() + p1.getH() > p2.getG() + p2.getH();
};

std::list<Puzzle> PathFinding::aStar(Puzzle &start) const
{
	PuzzlePriorityQueue<AStarCompareFunction> opened;
	ClosedSet                                 closed;
	CameFromMap                               cameFrom;

	Heuristic::init();
	Puzzle goal = Puzzle::getGoal();

	start.setG(0);
	start.setH();
	opened.push(start);

	while (!opened.empty())
	{
		Puzzle current = opened.top();

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
	}
	return {};
}

std::list<Puzzle> PathFinding::resolve(Puzzle &start, Algorithm algorithm) const
{
	if (algorithm == Algorithm::aStar)
		return this->aStar(start);
	if (algorithm == Algorithm::greedy)
		return this->greedy(start);
	if (algorithm == Algorithm::uniformCost)
		return this->uniformCost(start);
	return {};
}
