#include "Heuristic.class.hpp"
#include "Options.struct.hpp"
#include "Parser.class.hpp"
#include "Position.struct.hpp"
#include "Puzzle.class.hpp"
#include "utils.hpp"

#include <algorithm>
#include <boost/program_options.hpp>
#include <cstdlib>
#include <deque>
#include <exception>
#include <iostream>
#include <list>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef unsigned int uint;

namespace po = boost::program_options;

int getOptions(int argc, const char **argv, Options &options)
{
	std::string file;

	po::options_description desc(catArgs("Usage: ", argv[0], " [options]\nOptions"));
	desc.add_options()                                                                   //
		("help,h", "Produce help message")                                               //
		("parse-only,p", "Parse input and display the puzzle")                           //
		("file,f", po::value<std::string>(), "Path to the puzzle file (default stdin)"); //
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		return 0;
	}
	if (vm.count("file"))
		options.file = vm["file"].as<std::string>();
	options.parseOnly = static_cast<bool>(vm.count("parse-only"));
	return 1;
}

struct Compare
{
	bool operator()(const Puzzle &p1, const Puzzle &p2)
	{
		return (p1.getF() > p2.getF());
	}
};

class MyQueue: public std::priority_queue<Puzzle, std::vector<Puzzle>, Compare>
{
	public:
	bool hasValue(const Puzzle &puzzle) const
	{
		const Puzzle *addrStart = &this->top();
		const Puzzle *addrEnd = &this->top() + this->size();
		for (const Puzzle *it = addrStart; it != addrEnd; it++)
		{
			if (*it == puzzle)
				return true;
		}
		return false;
	}
};

struct HashFunction
{
	size_t operator()(const Puzzle &puzzle) const
	{
		return puzzle.getHash();
	}
};

std::list<Puzzle> getList(std::unordered_map<Puzzle, Puzzle, HashFunction> &cameFrom, const Puzzle &start,
                          const Puzzle &goal)
{
	std::list<Puzzle> solution;

	solution.push_front(goal);
	while (solution.front().getHash() != start.getHash())
		solution.push_front(cameFrom[solution.front()]);

	return solution;
}

std::list<Puzzle> process(Puzzle &start, const Options &options)
{
	MyQueue                                          opened;
	std::unordered_set<Puzzle, HashFunction>         closed;
	std::unordered_map<Puzzle, Puzzle, HashFunction> cameFrom;

	Heuristic::init(start.getSize());
	Puzzle::setHeuristicFunction(Heuristic::manhattan);
	Puzzle goal = Puzzle::getGoal(start.getSize());

	start.setG(0);
	start.updateParameters();
	opened.push(start);

	while (!opened.empty())
	{
		Puzzle current = opened.top();

		if (current == goal)
			return getList(cameFrom, start, goal);

		opened.pop();
		closed.insert(current);
		for (Puzzle &child: current.getChildren())
		{
			bool foundInOpened = opened.hasValue(child);
			bool foundInClosed = closed.find(child) != closed.end();
			if (!foundInOpened && !foundInClosed)
			{
				child.setG(current.getG() + 1);
				cameFrom[child] = current;
				opened.push(std::move(child));
			}
			else if (child.getG() > current.getG() + 1)
			{
				child.setG(current.getG() + 1);
				cameFrom[child] = current;
				if (foundInClosed)
				{
					closed.erase(child);
					opened.push(std::move(child));
				}
			}
		}
	}
	return {};
}

int main(int argc, char const *argv[])
{
	Options options;
	Puzzle  start;

	try
	{
		if (!getOptions(argc, argv, options))
			return 1;

		Parser parser(options.file);
		parser.parse();
		if (options.parseOnly)
		{
			parser.getPuzzle().print(std::cout, true);
			return 0;
		}
		start = parser.getPuzzle();
		std::cout << "START" << std::endl;
		std::cout << start << "-----" << std::endl << std::endl;
		std::list<Puzzle> list = process(start, options);
		for (const Puzzle &puzzle: list)
			std::cout << puzzle << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "\033[0;31mError:\033[0m" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
