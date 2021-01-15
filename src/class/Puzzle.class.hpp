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
	int                    _h;
	int                    _g;
	size_t                 _hash;

	static int (*heuristicFunction)(const Puzzle &);

	void   init(int size);
	size_t hash() const;

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
	bool                  operator==(const Puzzle &) const;
	bool                  operator!=(const Puzzle &) const;
	void                  print(std::ostream &os = std::cout, bool displaySize = false) const;
	void                  setZeroPosition(const Position &pos);
	void                  setZeroPosition();
	std::optional<Puzzle> move(Move direction) const;
	std::vector<Puzzle>   getChildren() const;
	int                   getH() const;
	void                  updateParameters();
	int                   getG() const;
	void                  setG(int value);
	int                   getF() const;
	size_t                getHash() const;

	static Puzzle getGoal(int size);
	static void   setHeuristicFunction(int (*heuristicFunction)(const Puzzle &));
};

std::ostream &operator<<(std::ostream &os, const Puzzle &puz);

#endif // !PUZZLE_HPP
