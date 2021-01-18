#ifndef PUZZLE_HPP
#define PUZZLE_HPP

#include "Move.enum.hpp"
#include "Position.struct.hpp"

#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <utility>

typedef unsigned int uint;

class Puzzle
{
	private:
	int      _size;
	size_t   _data;
	Position _emptyPos;
	int      _h;
	int      _g;

	static int (*heuristicFunction)(const Puzzle &);

	public:
	Puzzle();
	Puzzle(int size);
	Puzzle(int size, size_t data);
	Puzzle(const Puzzle &);
	Puzzle &operator=(const Puzzle &);
	Puzzle(Puzzle &&);
	Puzzle &operator=(Puzzle &&);
	~Puzzle();

	int                   getSize() const;
	size_t                getData() const;
	int                   operator[](int index) const;
	int                   getAt(int index) const;
	void                  setAt(int index, int value);
	int                   getAt(int y, int x) const;
	void                  setAt(int y, int x, int value);
	int                   getAt(const Position &pos) const;
	void                  setAt(const Position &pos, int value);
	bool                  operator==(const Puzzle &) const;
	bool                  operator!=(const Puzzle &) const;
	void                  swap(int index1, int index2);
	void                  swap(int y1, int x1, int y2, int x2);
	void                  swap(const Position &pos1, const Position &pos2);
	void                  print(std::ostream &os = std::cout, bool displaySize = false) const;
	void                  setZeroPosition(const Position &pos);
	void                  setZeroPosition();
	std::optional<Puzzle> move(Move direction) const;
	std::list<Puzzle>     getChildren() const;
	void                  updateParameters();
	int                   getH() const;
	void                  setH();
	int                   getG() const;
	void                  setG(int value);
	int                   getF() const;

	static Puzzle getGoal(int size);
	static void   setHeuristicFunction(int (*heuristicFunction)(const Puzzle &));
};

std::ostream &operator<<(std::ostream &os, const Puzzle &puz);

#endif // !PUZZLE_HPP
