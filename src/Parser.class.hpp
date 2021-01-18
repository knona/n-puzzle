#ifndef PARSER_HPP
#define PARSER_HPP

#include "Array.class.hpp"
#include "FilePosition.struct.hpp"
#include "Puzzle.class.hpp"

#include <fstream>
#include <optional>
#include <string>
#include <unordered_set>

class Parser
{
	private:
	const std::optional<std::string> &_file;
	std::ifstream                     _fileStream;

	Puzzle                  _puzzle;
	std::unordered_set<int> _hashTable;

	int          _rows;
	FilePosition _pos;

	public:
	Parser() = delete;
	Parser(const std::optional<std::string> &file);
	Parser(const Parser &) = delete;
	Parser &operator=(const Parser &) = delete;
	Parser(Parser &&) = delete;
	Parser &operator=(Parser &&) = delete;
	~Parser();

	void          isDigitStr(const std::string &str, int &index_end);
	int           strtoint(const std::string &str, size_t *index = nullptr, int max = std::numeric_limits<int>::max(),
	                       int min = 0);
	void          setPuzzleSize(const std::string &line, int &i);
	void          setPuzzleRow(const std::string &line, int &i);
	void          parseFromStream(std::istream &stream, std::string &line);
	std::istream &getInputStream();
	void          closeFile();
	void          parse();
	Puzzle &      getPuzzle();
};

#endif // !PARSER_HPP
