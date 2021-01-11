#ifndef PUZZLE_HPP
#define PUZZLE_HPP

#include "Position.hpp"
#include "main.hpp"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

class Puzzle
{
	private:
	uint                    _size;
	std::unique_ptr<uint[]> _data;

	void init(uint size);

	public:
	Puzzle() = delete;
	Puzzle(uint size);
	Puzzle(const Puzzle &);
	Puzzle &operator=(const Puzzle &);
	Puzzle(Puzzle &&);
	Puzzle &operator=(Puzzle &&);
	~Puzzle();

	uint *      operator[](uint index);
	const uint *operator[](uint index) const;
	uint &      at(uint a, uint b);
	uint        at(uint a, uint b) const;
	uint &      at(Position pos);
	uint        at(Position pos) const;
	void        print(std::ostream &os = std::cout) const;
};

std::ostream &operator<<(std::ostream &os, Puzzle &puz);

#endif // !PUZZLE_HPP
