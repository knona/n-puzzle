#ifndef PUZZLE_HPP
#define PUZZLE_HPP

#include "Move.enum.hpp"
#include "Position.struct.hpp"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

typedef unsigned int uint;

class Puzzle
{
	private:
	uint                    _size;
	std::unique_ptr<uint[]> _data;
	Position                _emptyPos;
	bool                    _isEmptyPosDefined;

	void init(uint size);

	public:
	Puzzle();
	Puzzle(uint size);
	Puzzle(const Puzzle &);
	Puzzle &operator=(const Puzzle &);
	Puzzle(Puzzle &&);
	Puzzle &operator=(Puzzle &&);
	~Puzzle();

	uint        getSize() const;
	uint *      operator[](uint index);
	const uint *operator[](uint index) const;
	uint &      at(uint a, uint b);
	uint        at(uint a, uint b) const;
	uint &      at(Position pos);
	uint        at(Position pos) const;
	void        print(std::ostream &os = std::cout) const;
	void        setZeroPosition(const Position &pos);
	void        setZeroPosition();
	void        move(Move direction);
};

std::ostream &operator<<(std::ostream &os, const Puzzle &puz);

#endif // !PUZZLE_HPP
