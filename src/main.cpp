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

class MyMap: public std::unordered_map<Puzzle, int, Puzzle::HashFunction>
{
	public:
	int &operator[](const Puzzle &k)
	{
		if (this->find(k) == this->end())

			this->std::unordered_map<Puzzle, int, Puzzle::HashFunction>::operator[](k) =
				std::numeric_limits<int>::max() / 2;

		return this->std::unordered_map<Puzzle, int, Puzzle::HashFunction>::operator[](k);
	}

	int &operator[](Puzzle &&k)
	{
		if (this->find(k) == this->end())

			this->std::unordered_map<Puzzle, int, Puzzle::HashFunction>::operator[](k) =
				std::numeric_limits<int>::max() / 2;

		return this->std::unordered_map<Puzzle, int, Puzzle::HashFunction>::operator[](k);
	}
};

Puzzle goal;

int getCost(int x1, int y1, int x2, int y2)
{
	return std::abs(y2 - y1) + abs(x2 - x1);
}

int getCost(Position pos1, Position pos2)
{
	return std::abs(pos2.y - pos1.y) + abs(pos2.x - pos1.x);
}

Array<Position> arr;

int h(const Puzzle &puzzle)
{
	int size = puzzle.getSize();
	int total = 0;

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			int cost = getCost(arr[puzzle.at(y, x)], { y, x });
			total += cost;
		}
	}
	return total;
}

// int h(const Puzzle &puzzle)
// {
// 	int   size = puzzle.getSize();
// 	Puzzle goal = Puzzle::getFinalState(size);
// 	int   total = 0;

// 	for (int i = 0; i < size * size; i++)
// 	{
// 		if (puzzle[i] != goal[i])
// 			total += 1;
// 	}

// 	return total;
// }

MyMap gScore;

struct Compare
{
	bool operator()(const Puzzle &p1, const Puzzle &p2)
	{
		return (h(p1) + gScore[p1]) > (h(p2) + gScore[p2]);
	}
};

class MyQueue: public std::priority_queue<Puzzle, std::vector<Puzzle>, Compare>
{
	public:
	bool checkValue(const Puzzle &puzzle) const
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

void init(int size)
{
	arr = Array<Position>(size * size);
	goal = Puzzle::getFinalState(size);
	for (int y = 0; y < size; y++)
		for (int x = 0; x < size; x++)
			arr[goal.at(y, x)] = { y, x };
}

Puzzle process(Puzzle &start, const Options &options)
{
	MyQueue                                          opened;
	std::unordered_set<Puzzle, Puzzle::HashFunction> closed;

	init(start.getSize());
	Puzzle finalSate = Puzzle::getFinalState(start.getSize());
	gScore[start] = 0;
	opened.push(start);

	while (!opened.empty())
	{
		Puzzle current = opened.top();

		if (current == finalSate)
			return finalSate;

		// std::cout << current << std::endl;

		opened.pop();
		// if (!opened.empty())
		// {
		// 	std::cout << "1=\n" << current << h(current) + gScore[current] << std::endl;
		// 	std::cout << "2=\n" << opened.top() << h(opened.top()) + gScore[opened.top()] << std::endl;
		// }
		closed.insert(current);
		for (const Puzzle &child: current.getChildren())
		{
			bool foundInOpened = opened.checkValue(child);
			bool foundInClosed = closed.find(child) != closed.end();
			if (!foundInOpened && !foundInClosed)
			{
				// std::cout << h(child) << " " << gScore[child] << std::endl;
				gScore[child] = gScore[current] + 1;
				opened.push(child);
				// predecesseor(s) <- e
			}
			else
			{
				if (gScore[child] > gScore[current] + 1)
				{
					gScore[child] = gScore[current] + 1;
					// predecesseor(s) <- e
					if (foundInClosed)
					{
						// std::cout << h(child) << " " << gScore[child] << std::endl;
						closed.erase(child);
						opened.push(child);
					}
				}
			}
		}
	}
	return Puzzle(start.getSize());
}

int main(int argc, char const *argv[])
{
	Options options;
	Puzzle  puzzle;

	try
	{
		if (!getOptions(argc, argv, options))
			return 1;

		Parser parser(options.file);
		parser.parse();
		if (options.parseOnly)
		{
			std::cout << parser.getPuzzle();
			return 0;
		}
		puzzle = parser.getPuzzle();
		std::cout << puzzle << std::endl;
		Puzzle goal = process(puzzle, options);
		std::cout << goal << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "\033[0;31mError:\033[0m" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
