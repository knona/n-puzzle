#ifndef PUZZLE_HPP
#define PUZZLE_HPP

#include "Move.enum.hpp"
#include "Position.struct.hpp"

#include <iostream>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

typedef unsigned int uint;

class Puzzle
{
	private:
	int                    _size;
	std::unique_ptr<int[]> _data;
	Position               _emptyPos;
	bool                   _isEmptyPosDefined;

	void init(int size);

	public:
	Puzzle();
	Puzzle(int size);
	Puzzle(const Puzzle &);
	Puzzle &operator=(const Puzzle &);
	Puzzle(Puzzle &&);
	Puzzle &operator=(Puzzle &&);
	~Puzzle();

	int                   getSize() const;
	int &                 operator[](int index);
	int                   operator[](int index) const;
	int &                 at(int index);
	int                   at(int index) const;
	int &                 at(int y, int x);
	int                   at(int y, int x) const;
	int &                 at(Position pos);
	int                   at(Position pos) const;
	void                  print(std::ostream &os = std::cout, bool displaySize = false) const;
	void                  setZeroPosition(const Position &pos);
	void                  setZeroPosition();
	void                  move(Move direction);
	std::optional<Puzzle> moveAndRet(Move direction) const;
	std::vector<Puzzle>   getChildren() const;

	bool operator==(const Puzzle &) const;
	bool operator!=(const Puzzle &) const;

	struct HashFunction
	{
		size_t operator()(const Puzzle &puzzle) const;
	};

	static Puzzle getFinalState(int size);
};

std::ostream &operator<<(std::ostream &os, const Puzzle &puz);

#endif // !PUZZLE_HPP
