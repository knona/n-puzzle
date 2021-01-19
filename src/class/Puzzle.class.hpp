#ifndef PUZZLE_HPP
#define PUZZLE_HPP

#include "Array.class.hpp"
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
	size_t   _data;
	Position _emptyPos;
	int      _h;
	int      _g;

	int getEmptyParity() const;
	int getPuzzleParity() const;

	static int             _size;
	static Puzzle          _goal;
	static Array<Position> _goalMap;
	static int (*_heuristicFunction)(const Puzzle &);

	static void setGoal();
	static void setGoalMap();

	public:
	Puzzle();
	Puzzle(size_t data);
	Puzzle(const Puzzle &);
	Puzzle &operator=(const Puzzle &);
	Puzzle(Puzzle &&);
	Puzzle &operator=(Puzzle &&);
	~Puzzle();

	size_t                getData() const;
	const Position        getEmptyPos() const;
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
	Position              find(int nb) const;
	Array<Position>       getPuzzleMap() const;
	void                  print(std::ostream &os = std::cout, bool displaySize = false) const;
	void                  setZeroPosition(const Position &pos);
	void                  setZeroPosition();
	std::optional<Puzzle> move(Move direction) const;
	std::list<Puzzle>     getChildren() const;
	int                   getH() const;
	void                  setH();
	int                   getG() const;
	void                  setG(int value);
	int                   getF() const;
	bool                  checkIsSolvable() const;

	static int                    getSize();
	static bool                   isSizeSet();
	static void                   setSize(int size);
	static const Puzzle &         getGoal();
	static const Array<Position> &getGoalMap();
	static void                   setHeuristicFunction(int (*heuristicFunction)(const Puzzle &));
};

std::ostream &operator<<(std::ostream &os, const Puzzle &puz);

#endif // !PUZZLE_HPP
