#include "Parser.class.hpp"

#include "Exception.class.hpp"
#include "utils.hpp"

#include <exception>
#include <iostream>

Parser::Parser(const std::optional<std::string> &file): _file(file), _rows(0), _pos { 1, 1 }, _isSizeSet(false)
{}

Parser::~Parser()
{
	this->closeFile();
}

std::istream &Parser::getInputStream()
{
	if (!this->_file.has_value())
		return std::cin;

	this->_fileStream.open(this->_file.value());
	if (!this->_fileStream.is_open())
		throw std::runtime_error(catArgs("File ", this->_file.value(), " cannot be openned"));
	return this->_fileStream;
}

void Parser::closeFile()
{
	if (this->_file.has_value() && this->_fileStream.is_open())
		this->_fileStream.close();
}

void Parser::isDigitStr(const std::string &str, int &indexEnd)
{
	int i = 0;
	int sign = 0;

	if (str[i] == '+' || str[i] == '-')
	{
		sign = 1;
		i++;
	}
	while (i < static_cast<int>(str.length()) && str[i] >= '0' && str[i] <= '9')
		i++;
	if (i == sign || (str[i] != '\0' && str[i] != '#' && !std::isspace(str[i])))
		throw Exception::ParserLight("Argument is not a valid number");
	indexEnd = i;
}

int Parser::strtoint(const std::string &str, size_t *index, int max, int min)
{
	int n;
	int indexEnd;

	this->isDigitStr(str, indexEnd);
	std::string_view nStr(str.c_str(), indexEnd);
	try
	{
		n = std::stoi(str, index);
	}
	catch (const std::out_of_range &e)
	{
		if (str[0] == '-')
			throw Exception::ParserLight(catArgs(nStr, " is lower than ", min));
		else
			throw Exception::ParserLight(catArgs(nStr, " is greater than ", max));
	}

	if (n < min)
		throw Exception::ParserLight(catArgs(nStr, " is lower than ", min));
	if (n > max)
		throw Exception::ParserLight(catArgs(nStr, " is greater than ", max));

	return n;
}

void Parser::setPuzzleSize(const std::string &line, int &i)
{
	size_t nbChars;
	int    size;

	this->_pos.col = i;
	size = static_cast<int>(this->strtoint(line.c_str() + i, &nbChars, 4, 3));
	for (i += nbChars; i < static_cast<int>(line.length()) && std::isspace(line[i]); i++)
		;
	if (i != static_cast<int>(line.length()) && line[i] != '#')
		throw Exception::ParserLight("Puzzle's size is required", false);
	this->_puzzle = Puzzle(size);
	this->_isSizeSet = true;
}

void Parser::setPuzzleRow(const std::string &line, int &i)
{
	int    cols = 0;
	int    n;
	int    size = this->_puzzle.getSize();
	size_t nbChars;

	if (size == this->_rows)
		throw Exception::ParserLight("The number of rows does not match the given size", false);
	for (; cols < size; cols++)
	{
		this->_pos.col = i;
		n = this->strtoint(line.c_str() + i, &nbChars, size * size - 1, 0);
		auto [it, isNewElement] = this->_hashTable.insert(n);
		if (!isNewElement)
			throw Exception::ParserLight("Number already set");
		this->_puzzle.setAt(this->_rows, cols, n);
		if (n == 0)
			this->_puzzle.setZeroPosition({ _rows, cols });
		for (i += nbChars; i < static_cast<int>(line.length()) && std::isspace(line[i]); i++)
			;
		if (i == static_cast<int>(line.length()) || line[i] == '#')
			break;
	}
	this->_pos.col = i;
	if (cols + 1 < size)
		throw Exception::ParserLight(catArgs("Not enough arguments, ", size, " numbers are required"));
	if (cols + 1 > size)
		throw Exception::ParserLight(catArgs("Too much arguments, ", size, " numbers are required"));
	this->_rows++;
}

void Parser::parseFromStream(std::istream &stream, std::string &line)
{
	int i;

	for (; std::getline(stream, line); this->_pos.line++)
	{
		for (i = 0; i < static_cast<int>(line.length()) && std::isspace(line[i]); i++)
			;
		if (i == static_cast<int>(line.length()) || line[i] == '#')
			continue;
		if (!this->_isSizeSet)
			this->setPuzzleSize(line, i);
		else
			this->setPuzzleRow(line, i);
	}
	if (!this->_isSizeSet)
		throw Exception::ParserLight("Invalid file", false);
	if (this->_puzzle.getSize() != this->_rows)
		throw Exception::ParserLight("The number of rows does not match the given size", false);
}

void Parser::parse()
{
	std::string line;

	try
	{
		this->parseFromStream(this->getInputStream(), line);
	}
	catch (const Exception::ParserLight &e)
	{
		if (e.getAddPosition())
			throw Exception::Parser(line, this->_pos, e.what());
		throw;
	}
	this->closeFile();
}

Puzzle &Parser::getPuzzle()
{
	return this->_puzzle;
}
