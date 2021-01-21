#include "CommandLine.class.hpp"

#include "Heuristic.class.hpp"
#include "Puzzle.class.hpp"
#include "utils.hpp"

#include <exception>
#include <iostream>

void CommandLine::setHeuristic(const po::variables_map &vm)
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

void CommandLine::setAlgorithm(const po::variables_map &vm, Options &options)
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

int CommandLine::getOptions(int argc, const char **argv, Options &options)
{
	std::string file;

	po::options_description desc(catArgs("Usage: ", argv[0], " [options]\nOptions"));
	desc.add_options()                                                                  //
		("help,h", "Produce help message")                                              //
		("file,f", po::value<std::string>(), "Path to the puzzle file (default stdin)") //
		("parse-only", "Parse input and display the puzzle")                            //
		("quiet,q", "Do not display solution on stdout")                                //
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
	options.quiet = static_cast<bool>(vm.count("quiet"));
	setHeuristic(vm);
	setAlgorithm(vm, options);
	return 1;
}
