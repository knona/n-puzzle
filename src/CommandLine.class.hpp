#ifndef COMMAND_LINE_HPP
#define COMMAND_LINE_HPP

#include "Options.struct.hpp"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

class CommandLine
{
	private:
	static void setHeuristic(const po::variables_map &vm);
	static void setAlgorithm(const po::variables_map &vm, Options &options);

	public:
	static int getOptions(int argc, const char **argv, Options &options);
};

#endif // !COMMAND_LINE_HPP
