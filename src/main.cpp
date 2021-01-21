#include "Gui.class.hpp"
#include "Heuristic.class.hpp"
#include "Options.struct.hpp"
#include "Parser.class.hpp"
#include "PathFinding.class.hpp"
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
#include <vector>

namespace po = boost::program_options;
namespace chrono = std::chrono;

void setHeuristic(const po::variables_map &vm)
{
	if (vm["heuristic"].as<std::string>() == "manhattan")
		Puzzle::setHeuristicFunction(Heuristic::manhattan);
	else if (vm["heuristic"].as<std::string>() == "hamming")
		Puzzle::setHeuristicFunction(Heuristic::hamming);
	else if (vm["heuristic"].as<std::string>() == "linear")
		Puzzle::setHeuristicFunction(Heuristic::linearConflicts);
	else
		throw std::runtime_error("Please enter a proper heuristic [manhattan/linear/hamming]");
}

void setAlgorithm(const po::variables_map &vm, Options &options)
{
	if (vm["algorithm"].as<std::string>() == "astar")
		options.algorithm = PathFinding::Algorithm::aStar;
	else if (vm["algorithm"].as<std::string>() == "greedy")
		options.algorithm = PathFinding::Algorithm::greedy;
	else if (vm["algorithm"].as<std::string>() == "uniform")
		options.algorithm = PathFinding::Algorithm::uniformCost;
	else
		throw std::runtime_error("Please enter a proper algorithm [astar/greedy/uniform]");
}

int getOptions(int argc, const char **argv, Options &options)
{
	std::string file;

	po::options_description desc(catArgs("Usage: ", argv[0], " [options]\nOptions"));
	desc.add_options()                                                                  //
		("help,h", "Produce help message")                                              //
		("parse-only", "Parse input and display the puzzle")                            //
		("file,f", po::value<std::string>(), "Path to the puzzle file (default stdin)") //
		("heuristic", po::value<std::string>()->default_value("linear"),                //
	     "Heuristic to use :[manhattan/linear/hamming] (default linear)")               //
		("algorithm", po::value<std::string>()->default_value("astar"),                 //
	     "Algorithm to use :[astar/greedy/uniform] (default astar)")                    //
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
	setHeuristic(vm);
	setAlgorithm(vm, options);
	return 1;
}

void printDuration(chrono::duration<double, std::milli> fpMs)
{
	auto h = chrono::duration_cast<chrono::hours>(fpMs);
	auto m = chrono::duration_cast<chrono::minutes>(fpMs -= h);
	auto s = chrono::duration_cast<chrono::seconds>(fpMs -= m);
	auto ms = chrono::duration_cast<chrono::milliseconds>(fpMs -= s);
	std::cout << "Computed time to solution:" << h.count() << " hours, " << m.count() << " minutes, " << s.count()
			  << " seconds, " << ms.count() << " milliseconds\n";
}

int main(int argc, char const *argv[])
{
	Options     options;
	Puzzle      start;
	PathFinding pathFinding;

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
		auto              t1 = chrono::high_resolution_clock::now();
		std::list<Puzzle> list = pathFinding.resolve(start, options.algorithm);
		auto              t2 = chrono::high_resolution_clock::now();
		std::cout << "\033[0;32mFinished...\033[0m" << std::endl;
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
		std::cin.ignore(std::numeric_limits<int>::max());
		std::cerr << "\033[0;31mError:\033[0m" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
