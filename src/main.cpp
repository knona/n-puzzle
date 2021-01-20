#include "Gui.class.hpp"
#include "Heuristic.class.hpp"
#include "Options.struct.hpp"
#include "Parser.class.hpp"
#include "Position.struct.hpp"
#include "Puzzle.class.hpp"
#include "PuzzlePriorityQueue.class.hpp"
#include "utils.hpp"

#include <boost/program_options.hpp>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef unsigned int uint;

namespace po = boost::program_options;
namespace chrono = std::chrono;

int getOptions(int argc, const char **argv, Options &options)
{
	std::string file;

	po::options_description desc(catArgs("Usage: ", argv[0], " [options]\nOptions"));
	desc.add_options()                                                                  //
		("help,h", "Produce help message")                                              //
		("parse-only,p", "Parse input and display the puzzle")                          //
		("file,f", po::value<std::string>(), "Path to the puzzle file (default stdin)") //
		("gui,g", "Enable the gui");                                                    //
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
	options.enableGui = static_cast<bool>(vm.count("gui"));
	return 1;
}

std::list<Puzzle> getList(std::unordered_map<size_t, size_t> &cameFrom, const Puzzle &start, const Puzzle &goal)
{
	std::list<Puzzle> solution;

	solution.push_front(goal);
	while (solution.front() != start)
		solution.push_front(Puzzle(cameFrom[solution.front().getData()]));

	return solution;
}

std::list<Puzzle> process(Puzzle &start, const Options &options)
{
	PuzzlePriorityQueue                opened;
	std::unordered_set<size_t>         closed;
	std::unordered_map<size_t, size_t> cameFrom;

	Heuristic::init();
	Puzzle::setHeuristicFunction(Heuristic::linear_conflicts);
	Puzzle goal = Puzzle::getGoal();

	start.setG(0);
	start.setH();
	opened.push(start);

	while (!opened.empty())
	{
		Puzzle current = opened.top();

		if (current == goal)
			return getList(cameFrom, start, goal);

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

void printDuration(chrono::duration<double, std::milli> fp_ms)
{
    auto h = chrono::duration_cast<chrono::hours>(fp_ms);
    auto m = chrono::duration_cast<chrono::minutes>(fp_ms -= h);
    auto s = chrono::duration_cast<chrono::seconds>(fp_ms -= m);
    auto ms = chrono::duration_cast<chrono::milliseconds>(fp_ms -= s);
    std::cout << "Computed time to solution:"
		<< h.count() << " hours, "
        << m.count() << " minutes, "
        << s.count() << " seconds, "
        << ms.count() << " milliseconds\n";
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
		if (!start.checkIsSolvable())
			throw std::runtime_error("The puzzle is unsolvable");
		std::cout << "\033[0;33mProcessing...\033[0m" << std::endl;
		auto t1 = chrono::high_resolution_clock::now();
		std::list<Puzzle> list = process(start, options);
		auto t2 = chrono::high_resolution_clock::now();
		std::cout << "\033[0; Finished...\033[0m" << std::endl;
		printDuration(t2 - t1);
		if (options.enableGui)
		{
			Gui gui;
			gui.init();
			gui.render(std::vector<Puzzle>(list.begin(), list.end()));
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "\033[0;31mError:\033[0m" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
