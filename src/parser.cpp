#include "Exception.class.hpp"
#include "Puzzle.class.hpp"
#include "cat-args.template.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void isDigitStr(const std::string &str, uint &index_end)
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
	if (i == sign || (str[i] != '\0' && str[i] != '#' && !std::isspace(str[i])))
		throw Exception::ParserLight("Argument is not a valid number");
	index_end = i;
}

uint strtou(const std::string &str, size_t *index = nullptr, uint max = std::numeric_limits<uint>::max(), uint min = 0)
{
	unsigned long     n;
	uint              index_end;
	std::stringstream errorStream;

	isDigitStr(str, index_end);
	std::string_view n_str(str.c_str(), index_end);
	try
	{
		n = std::stoi(str, index);
	}
	catch (const std::out_of_range &e)
	{
		throw Exception::ParserLight(catArgs(n_str, " is greater than ", max));
	}

	if (n < min)
		throw Exception::ParserLight(catArgs(n_str, " is lower than ", min));
	if (n > max)
	{
		if (str[0] == '-')
			throw Exception::ParserLight(catArgs(n_str, " is lower than ", min));
		else
			throw Exception::ParserLight(catArgs(n_str, " is greater than ", max));
	}

	return static_cast<uint>(n);
}

uint getPuzzleSize(const std::string &line, uint i, uint &colError)
{
	size_t nbChars;
	uint   n;

	colError = i;
	n = strtou(line.c_str() + i, &nbChars, 100, 3);
	for (i += nbChars; i < line.length() && std::isspace(line[i]); i++)
		;
	if (i != line.length() && line[i] != '#')
		throw Exception::ParserLight("Puzzle's size is required");
	return n;
}

void getPuzzleRow(const std::string &line, uint i, uint &colError, bool &hasZero, Puzzle &puzzle)
{
	uint                     size = puzzle.getSize();
	static std::vector<bool> hashTable(size * size, false);
	static uint              y = 0;
	uint                     x = 0;
	uint                     n;
	size_t                   nbChars;
	std::stringstream        errorStream;

	for (; x < size; x++)
	{
		colError = i;
		n = strtou(line.c_str() + i, &nbChars, size * size - 1, 0);
		if (size == y)
			throw Exception::ParserLight("The number of rows does not match the given size", false);
		if (n == 0)
		{
			if (hasZero)
				throw Exception::ParserLight("Multiple zero numbers");
			hasZero = true;
		}
		if (hashTable[n])
			throw Exception::ParserLight("Number already set");
		hashTable[n] = true;
		puzzle.at(y, x) = n;
		for (i += nbChars; i < line.length() && std::isspace(line[i]); i++)
			;
		if (i == line.length() || line[i] == '#')
			break;
	}
	colError = i;
	if (x + 1 < size)
		throw Exception::ParserLight(catArgs("Not enough arguments, ", size, " numbers are required"));
	if (x + 1 > size)
		throw Exception::ParserLight(catArgs("Too much arguments, ", size, " numbers are required"));
	y++;
}

Puzzle parserFromStream(std::istream &stream, std::string &line, uint &lineCount, uint &colError)
{
	Puzzle puzzle;
	bool   hasZero = false;
	uint   nbRows = 0;
	uint   i;

	while (std::getline(stream, line))
	{
		for (i = 0; i < line.length() && std::isspace(line[i]); i++)
			;
		if (i == line.length() || line[i] == '#')
		{
			lineCount++;
			continue;
		}
		if (puzzle.getSize() == 0)
			puzzle = Puzzle(getPuzzleSize(line, i, colError));
		else
		{
			getPuzzleRow(line, i, colError, hasZero, puzzle);
			nbRows++;
		}
		lineCount++;
	}
	if (puzzle.getSize() == 0)
		throw Exception::ParserLight("Invalid file", false);
	if (puzzle.getSize() != nbRows)
		throw Exception::ParserLight("The number of rows does not match the given size", false);
	if (!hasZero)
		throw Exception::ParserLight("A zero number is required", false);
	return puzzle;
}

std::istream &streamFromFile(const char *file)
{
	static std::ifstream stream(file);

	if (!stream.is_open())
		throw std::runtime_error(catArgs("File ", file, " cannot be openned"));
	return stream;
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
	uint          lineCount = 0;
	uint          colError = 0;
	Puzzle        puzzle;

	try
	{
		puzzle = parserFromStream(stream, line, lineCount, colError);
	}
	catch (const Exception::ParserLight &e)
	{
		closeFile(stream, file);
		if (e.getAddPosition())
			throw Exception::Parser(line, lineCount, colError, e.what());
		throw;
	}
	closeFile(stream, file);
	return puzzle;
}
