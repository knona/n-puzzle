#ifndef PATH_FINDING_HPP
#define PATH_FINDING_HPP

#include "Puzzle.class.hpp"

#include <chrono>
#include <unordered_map>
#include <unordered_set>

typedef std::unordered_set<size_t>         ClosedSet;
typedef std::unordered_map<size_t, size_t> CameFromMap;

class PathFinding
{
	private:
	struct AStarCompareFunction
	{
		bool operator()(const Puzzle &p1, const Puzzle &p2);
	};

	struct GreedyCompareFunction
	{
		bool operator()(const Puzzle &p1, const Puzzle &p2);
	};

	struct UniformCostCompareFunction
	{
		bool operator()(const Puzzle &p1, const Puzzle &p2);
	};

	unsigned int _timeComplexity;
	unsigned int _sizeComplexity;

	std::list<Puzzle> getList(CameFromMap &cameFrom, const Puzzle &start, const Puzzle &goal) const;
	std::list<Puzzle> aStar(Puzzle &start);
	std::list<Puzzle> greedy(Puzzle &start);
	std::list<Puzzle> uniformCost(Puzzle &start);
	void              printDuration(std::chrono::duration<double, std::milli> fpMs) const;

	public:
	enum class Algorithm { aStar, greedy, uniformCost };

	PathFinding();
	PathFinding(const PathFinding &) = delete;
	PathFinding &operator=(const PathFinding &) = delete;
	PathFinding(PathFinding &&) = delete;
	PathFinding &operator=(PathFinding &&) = delete;
	~PathFinding();

	std::list<Puzzle> resolve(Puzzle &start, Algorithm algorithm);
};

#endif // !PATH_FINDING_HPP
