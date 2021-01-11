#include "Exception.hpp"
#include "Puzzle.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

// max must be lower or equal to max int
// min must be greater or equal to 0
uint strtou(const std::string &str, int max, int min, size_t *index = 0)
{
	int               nb;
	std::stringstream sstream;

	try
	{
		nb = std::stoi(str, index);
	}
	catch (const std::out_of_range &e)
	{
		if (str[0] == '-')
			sstream << nb << " is lower than " << min;
		else
			sstream << nb << " is greater than " << max;
		throw Exception::ParserLight(sstream.str());
	}
	catch (const std::invalid_argument &e)
	{
		throw Exception::ParserLight("Argument is not a valid number");
	}

	if (nb < min)
	{
		sstream << nb << " is lower than " << min;
		throw Exception::ParserLight(sstream.str());
	}
	if (nb > max)
	{
		sstream << nb << " is greater than " << max;
		throw Exception::ParserLight(sstream.str());
	}

	return static_cast<uint>(nb);
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
	start = i;
	size_t read;
	uint   nb = strtou(line.c_str() + i, 100, 3, &read);

	for (i += read; i < line.length() && std::isspace(line[i]); i++)
		;
	if (i != line.length() && line[i] != '#')
		throw Exception::ParserLight("Size of puzzle is required");
	return nb;
}

void getPuzzleRaw(const std::string &line, uint i, uint &start, bool &hasZero, Puzzle &puz)
{
	static uint rawNb = 0;
	uint        size = puz.getSize();
	uint        x = 0;
	size_t      read;

	for (; x < size; x++)
	{
		start = i;
		uint nb = strtou(line.c_str() + i, std::numeric_limits<int>::max(), 0, &read);

		if (nb == 0)
		{
			if (hasZero)
				throw Exception::ParserLight("Multiple zero");
			hasZero = true;
		}

		puz.at(rawNb, x) = nb;
		for (i += read; i < line.length() && std::isspace(line[i]); i++)
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
	rawNb++;
}

void processLine(const std::string &line, uint lineCount, uint &start, bool &hasZero, Puzzle &puz)
{
	uint i;

	for (i = 0; i < line.length() && std::isspace(line[i]); i++)
		;
	if (i == line.length() || line[i] == '#')
	{
		lineCount++;
		return;
	}
	if (puz.getSize() == 0)
		puz = Puzzle(getPuzzleSize(line, i, start));
	else
		getPuzzleRaw(line, i, start, hasZero, puz);
	lineCount++;
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
	Puzzle        puz;
	bool          hasZero = false;
	uint          lineCount = 0;
	uint          start = 0;

	while (std::getline(stream, line))
	{
		try
		{
			processLine(line, lineCount, start, hasZero, puz);
		}
		catch (const Exception::ParserLight &e)
		{
			closeFile(stream, file);
			throw Exception::Parser(line, lineCount, start, e.what());
		}
		lineCount++;
	}
	closeFile(stream, file);
	if (puz.getSize() == 0)
		throw Exception::ParserLight("Invalid file");
	if (!hasZero)
		throw Exception::ParserLight("No zero");
	return puz;
}
