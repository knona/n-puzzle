#include "Options.struct.hpp"
#include "Parser.class.hpp"
#include "Position.struct.hpp"
#include "Puzzle.class.hpp"
#include "utils.hpp"

#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <optional>
#include <sstream>

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

void process(Puzzle &puzzle, const Options &options)
{}

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
		process(parser.getPuzzle(), options);
	}
	catch (const std::exception &e)
	{
		std::cerr << "\033[0;31mError:\033[0m" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
