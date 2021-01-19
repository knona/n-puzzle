#include "Puzzle.class.hpp"

#include "utils.hpp"

#include <cstdio>
#include <exception>
#include <iomanip>
#include <utility>

int (*Puzzle::_heuristicFunction)(const Puzzle &) = nullptr;
int             Puzzle::_size = 0;
Puzzle          Puzzle::_goal;
Array<Position> Puzzle::_goalMap;

Puzzle::Puzzle(): _data(0), _emptyPos { -1, -1 }, _g(std::numeric_limits<int>::max())
{}

Puzzle::Puzzle(size_t data): _data(data), _g(std::numeric_limits<int>::max())
{
	this->setZeroPosition();
}

Puzzle::Puzzle(const Puzzle &puzzle)
{
	*this = puzzle;
}

Puzzle::Puzzle(Puzzle &&puzzle)
{
	*this = std::move(puzzle);
}

Puzzle &Puzzle::operator=(const Puzzle &puzzle)
{
	if (this != &puzzle)
	{
		this->_data = puzzle._data;
		this->_emptyPos = puzzle._emptyPos;
		this->_h = puzzle._h;
		this->_g = puzzle._g;
	}
	return *this;
}

Puzzle &Puzzle::operator=(Puzzle &&puzzle)
{
	if (this != &puzzle)
	{
		this->_data = puzzle._data;
		this->_emptyPos = puzzle._emptyPos;
		this->_h = puzzle._h;
		this->_g = puzzle._g;
	}
	return *this;
}

Puzzle::~Puzzle()
{}

int Puzzle::getSize()
{
	return Puzzle::_size;
}

void Puzzle::setSize(int size)
{
	if (size < 3 || size > 4)
		throw std::runtime_error("The puzzle's size must be between 3 and 4");
	Puzzle::_size = size;
	Puzzle::setGoal();
	Puzzle::setGoalMap();
}

bool Puzzle::isSizeSet()
{
	return Puzzle::_size != 0;
}

size_t Puzzle::getData() const
{
	return this->_data;
}

const Position Puzzle::getEmptyPos() const
{
	return this->_emptyPos;
}

int Puzzle::operator[](int index) const
{
	if (index >= Puzzle::_size * Puzzle::_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	return (this->_data >> (index * 4)) & 0xF;
}

int Puzzle::getAt(int index) const
{
	if (index >= Puzzle::_size * Puzzle::_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	return (this->_data >> (index * 4)) & 0xF;
}

void Puzzle::setAt(int index, int value)
{
	if (index >= Puzzle::_size * Puzzle::_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	this->_data &= ~(0xFL << (index * 4));
	this->_data |= (static_cast<size_t>(value) << (index * 4));
}

int Puzzle::getAt(int y, int x) const
{
	if (y >= Puzzle::_size)
		throw std::out_of_range(catArgs(y, " is out of range"));
	if (x >= Puzzle::_size)
		throw std::out_of_range(catArgs(x, " is out of range"));
	return (this->_data >> ((y * Puzzle::_size + x) * 4)) & 0xF;
}

void Puzzle::setAt(int y, int x, int value)
{
	if (y >= Puzzle::_size)
		throw std::out_of_range(catArgs(y, " is out of range"));
	if (x >= Puzzle::_size)
		throw std::out_of_range(catArgs(x, " is out of range"));
	this->_data &= ~(0xFL << ((y * Puzzle::_size + x) * 4));
	this->_data |= (static_cast<size_t>(value) << ((y * Puzzle::_size + x) * 4));
}

int Puzzle::getAt(const Position &pos) const
{
	if (pos.y >= Puzzle::_size)
		throw std::out_of_range(catArgs(pos.y, " is out of range"));
	if (pos.x >= Puzzle::_size)
		throw std::out_of_range(catArgs(pos.x, " is out of range"));
	return (this->_data >> ((pos.y * Puzzle::_size + pos.x) * 4)) & 0xF;
}

void Puzzle::setAt(const Position &pos, int value)
{
	if (pos.y >= Puzzle::_size)
		throw std::out_of_range(catArgs(pos.y, " is out of range"));
	if (pos.x >= Puzzle::_size)
		throw std::out_of_range(catArgs(pos.x, " is out of range"));
	this->_data &= ~(0xFL << ((pos.y * Puzzle::_size + pos.x) * 4));
	this->_data |= (static_cast<size_t>(value) << ((pos.y * Puzzle::_size + pos.x) * 4));
}

bool Puzzle::operator==(const Puzzle &puzzle) const
{
	return this->_data == puzzle._data;
}
bool Puzzle::operator!=(const Puzzle &puzzle) const
{
	return this->_data != puzzle._data;
}

void Puzzle::swap(int index1, int index2)
{
	int tmp = this->getAt(index1);
	this->setAt(index1, this->getAt(index2));
	this->setAt(index2, tmp);
}

void Puzzle::swap(int y1, int x1, int y2, int x2)
{
	int tmp = this->getAt(y1, x1);
	this->setAt(y1, x1, this->getAt(y2, x2));
	this->setAt(y2, x2, tmp);
}

void Puzzle::swap(const Position &pos1, const Position &pos2)
{
	int tmp = this->getAt(pos1);
	this->setAt(pos1, this->getAt(pos2));
	this->setAt(pos2, tmp);
}

Position Puzzle::find(int nb) const
{
	int size = Puzzle::getSize();

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			if (this->getAt(y, x) == nb)
				return { y, x };
		}
	}
	throw std::logic_error(catArgs("Number ", nb, " not found in puzzle"));
}

Array<Position> Puzzle::getPuzzleMap() const
{
	int             size = Puzzle::getSize();
	Array<Position> map(size * size);

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
			map[this->getAt(y, x)] = { y, x };
	}
	return map;
}

void Puzzle::print(std::ostream &os, bool displaySize) const
{
	int width = std::to_string(Puzzle::_size * Puzzle::_size - 1).length();

	if (displaySize)
		os << Puzzle::_size << std::endl;
	for (int i = 0; i < Puzzle::_size; i++)
	{
		for (int j = 0; j < Puzzle::_size; j++)
		{
			if (j != 0)
				os << " ";
			os << std::setw(width) << this->getAt(i, j);
		}
		os << std::endl;
	}
}

std::ostream &operator<<(std::ostream &os, const Puzzle &puz)
{
	puz.print(os);
	return os;
}

void Puzzle::setZeroPosition(const Position &pos)
{
	this->_emptyPos = pos;
}

void Puzzle::setZeroPosition()
{
	for (int y = 0; y < Puzzle::_size; y++)
	{
		for (int x = 0; x < Puzzle::_size; x++)
		{
			if (this->getAt(y, x) == 0)
			{
				this->_emptyPos.y = y;
				this->_emptyPos.x = x;
				return;
			}
		}
	}
	throw std::logic_error("0 value not found to define empty position");
}

std::optional<Puzzle> Puzzle::move(Move direction) const
{
	if (this->_emptyPos.y == -1 || this->_emptyPos.x == -1)
		throw std::logic_error("Cannot move, empty position is not properly set");

	Position newPos = this->_emptyPos;

	if (direction == Move::Top)
		newPos.y++;
	else if (direction == Move::Right)
		newPos.x--;
	else if (direction == Move::Bottom)
		newPos.y--;
	else if (direction == Move::Left)
		newPos.x++;

	if (newPos.y >= Puzzle::_size || newPos.y < 0 || newPos.x >= Puzzle::_size || newPos.x < 0)
		return {};

	Puzzle newPuzzle(*this);
	newPuzzle.swap(newPuzzle._emptyPos, newPos);
	newPuzzle._emptyPos = newPos;
	return newPuzzle;
}

std::list<Puzzle> Puzzle::getChildren() const
{
	std::list<Puzzle> children;

	for (int i = 0; i < 4; i++)
	{
		std::optional<Puzzle> puzzle = this->move(static_cast<Move>(i));
		if (puzzle.has_value())
			children.push_back(std::move(puzzle.value()));
	}
	return children;
}

void Puzzle::setGoal()
{
	int dir = 0;
	int nb = Puzzle::_size;
	int v = 1;
	int x = -1;
	int y = 0;

	for (; v != Puzzle::_size * Puzzle::_size; nb--)
	{
		for (int i = nb == Puzzle::_size || nb == 1 ? 1 : 0; i < 2; i++)
		{
			for (int j = 0; j < nb; j++)
			{
				if (dir == 0)
					x++;
				if (dir == 1)
					y++;
				if (dir == 2)
					x--;
				if (dir == 3)
					y--;
				Puzzle::_goal.setAt(y, x, v);
				v++;
			}
			dir = (dir + 1) % 4;
		}
	}
}

const Puzzle &Puzzle::getGoal()
{
	return Puzzle::_goal;
}

void Puzzle::setGoalMap()
{
	Puzzle::_goalMap = Puzzle::_goal.getPuzzleMap();
}

const Array<Position> &Puzzle::getGoalMap()
{
	return Puzzle::_goalMap;
}

int Puzzle::getH() const
{
	return this->_h;
}

void Puzzle::setH()
{
	this->_h = Puzzle::_heuristicFunction ? Puzzle::_heuristicFunction(*this) : 0;
}

int Puzzle::getG() const
{
	return this->_g;
}

void Puzzle::setG(int value)
{
	this->_g = value;
}

int Puzzle::getF() const
{
	return this->_g + this->_h;
}

void Puzzle::setHeuristicFunction(int (*heuristicFunction)(const Puzzle &))
{
	Puzzle::_heuristicFunction = heuristicFunction;
}

int Puzzle::getEmptyParity() const
{
	const Array<Position> &goalMap = Puzzle::getGoalMap();
	const Position &       emptyPos = this->getEmptyPos();
	const Position &       goalPos = goalMap[0];
	return (std::abs(goalPos.y - emptyPos.y) + abs(goalPos.x - emptyPos.x)) % 2;
}

int Puzzle::getPuzzleParity() const
{
	const Array<Position> &goalMap = Puzzle::getGoalMap();
	Puzzle                 puzzle(*this);
	int                    size = Puzzle::getSize();
	int                    nbTransitions = 0;

	for (int i = 0; i < size * size; i++)
	{
		Position        puzzlePos = puzzle.find(i);
		const Position &goalPos = goalMap[i];
		if (i != puzzle.getAt(goalPos))
		{
			puzzle.swap(puzzlePos, goalPos);
			nbTransitions++;
		}
	}
	return nbTransitions % 2;
}

bool Puzzle::checkIsSolvable() const
{
	return Puzzle::getEmptyParity() == Puzzle::getPuzzleParity();
}
