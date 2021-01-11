#include "Exception.hpp"
#include "Puzzle.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

void isDigitStr(const std::string &str)
{
	uint i = 0;
	uint sign = 0;

	if (str[i] == '+' || str[i] == '-')
	{
		sign = 1;
		i++;
	}
	while (i < str.length() && str[i] >= '0' && str[i] <= '9')
		i++;
	if (i == sign || (str[i] != '\0' && !std::isspace(str[i])))
		throw Exception::ParserLight("Argument is not a valid number");
}

// max must be lower or equal to max int
// min must be greater or equal to 0
uint strtou(const std::string &str, int max, int min, size_t *index = 0)
{
	int               n;
	std::stringstream sstream;

	isDigitStr(str);
	try
	{
		n = std::stoi(str, index);
	}
	catch (const std::out_of_range &e)
	{
		if (str[0] == '-')
			sstream << n << " is lower than " << min;
		else
			sstream << n << " is greater than " << max;
		throw Exception::ParserLight(sstream.str());
	}

	if (n < min)
	{
		sstream << n << " is lower than " << min;
		throw Exception::ParserLight(sstream.str());
	}
	if (n > max)
	{
		sstream << n << " is greater than " << max;
		throw Exception::ParserLight(sstream.str());
	}

	return static_cast<uint>(n);
}

std::istream &streamFromFile(const char *file)
{
	static std::ifstream stream(file);

	if (!stream.is_open())
		throw std::runtime_error(std::string("File ") + file + " cannot be openned");
	return stream;
}

uint getPuzzleSize(const std::string &line, uint i, uint &start)
{
	size_t nbChars;
	uint   n;

	start = i;
	n = strtou(line.c_str() + i, 100, 3, &nbChars);
	for (i += nbChars; i < line.length() && std::isspace(line[i]); i++)
		;
	if (i != line.length() && line[i] != '#')
		throw Exception::ParserLight("Puzzle' size is required");
	return n;
}

void getPuzzleRaw(const std::string &line, uint i, uint &start, bool &hasZero, Puzzle &puzzle)
{
	static uint y = 0;
	uint        x = 0;
	uint        n;
	uint        size = puzzle.getSize();
	size_t      nbChars;

	for (; x < size; x++)
	{
		start = i;
		n = strtou(line.c_str() + i, std::numeric_limits<int>::max(), 0, &nbChars);
		if (n == 0)
		{
			if (hasZero)
				throw Exception::ParserLight("Multiple zero numbers");
			hasZero = true;
		}
		puzzle.at(y, x) = n;
		for (i += nbChars; i < line.length() && std::isspace(line[i]); i++)
			;
		if (i == line.length() || line[i] == '#')
			break;
	}
	x++;
	start = i;
	if (x < size)
		throw Exception::ParserLight(std::string("Too few numbers, ") + std::to_string(size) + " are required");
	if (x > size)
		throw Exception::ParserLight(std::string("Too much numbers, ") + std::to_string(size) + " are required");
	y++;
}

void processLine(const std::string &line, uint &start, bool &hasZero, Puzzle &puzzle)
{
	uint i;

	for (i = 0; i < line.length() && std::isspace(line[i]); i++)
		;
	if (i == line.length() || line[i] == '#')
		return;
	if (puzzle.getSize() == 0)
		puzzle = Puzzle(getPuzzleSize(line, i, start));
	else
		getPuzzleRaw(line, i, start, hasZero, puzzle);
}

void closeFile(std::istream &stream, const char *file)
{
	if (file)
		static_cast<std::ifstream *>(&stream)->close();
}

Puzzle parser(const char *file)
{
	std::istream &stream = file ? streamFromFile(file) : std::cin;
	std::string   line;
	Puzzle        puzzle;
	bool          hasZero = false;
	uint          lineCount = 0;
	uint          start = 0;

	while (std::getline(stream, line))
	{
		try
		{
			processLine(line, start, hasZero, puzzle);
		}
		catch (const Exception::ParserLight &e)
		{
			closeFile(stream, file);
			throw Exception::Parser(line, lineCount, start, e.what());
		}
		lineCount++;
	}
	closeFile(stream, file);
	if (puzzle.getSize() == 0)
		throw Exception::ParserLight("Invalid file");
	if (!hasZero)
		throw Exception::ParserLight("A zero number is required");
	return puzzle;
}
