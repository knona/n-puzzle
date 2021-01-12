#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "main.hpp"

#include <exception>
#include <sstream>
#include <string>

class Exception
{
	public:
	static std::string error;

	class GameOver: public std::exception
	{
		public:
		virtual const char *what() const throw();
	};

	class Parser: public std::exception
	{
		public:
		Parser() = delete;
		Parser(const std::string line, uint lineNb, uint col, const std::string &detail);
		virtual const char *what() const throw();
	};

	class ParserLight: public std::exception
	{
		private:
		bool _addPosition;

		public:
		ParserLight(const std::string &detail, bool addPosition = true);
		virtual const char *what() const throw();
		bool                getAddPosition() const;
	};
};

#endif // !EXCEPTION_HPP
