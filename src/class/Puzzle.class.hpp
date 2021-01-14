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

	uint                  getSize() const;
	uint &                operator[](uint index);
	uint                  operator[](uint index) const;
	uint &                at(uint a, uint b);
	uint                  at(uint a, uint b) const;
	uint &                at(Position pos);
	uint                  at(Position pos) const;
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

	static Puzzle getFinalState(uint size);
};

std::ostream &operator<<(std::ostream &os, const Puzzle &puz);

#endif // !PUZZLE_HPP
