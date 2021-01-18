#include "Puzzle.class.hpp"

#include "utils.hpp"

#include <cstdio>
#include <exception>
#include <iomanip>
#include <utility>

int (*Puzzle::heuristicFunction)(const Puzzle &) = nullptr;

Puzzle::Puzzle(): _size(3), _data(0), _emptyPos { -1, -1 }, _g(std::numeric_limits<int>::max())
{}

Puzzle::Puzzle(int size): _size(size), _data(0), _emptyPos { -1, -1 }, _g(std::numeric_limits<int>::max())
{
	if (size < 3 || size > 4)
		throw std::runtime_error("The puzzle's size must be between 3 and 4");
}

Puzzle::Puzzle(int size, size_t data): _size(size), _data(data), _g(std::numeric_limits<int>::max())
{
	if (size < 3 || size > 4)
		throw std::runtime_error("The puzzle's size must be between 3 and 4");
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
		this->_size = puzzle._size;
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
		this->_size = puzzle._size;
		this->_emptyPos = puzzle._emptyPos;
		this->_h = puzzle._h;
		this->_g = puzzle._g;
	}
	return *this;
}

Puzzle::~Puzzle()
{}

int Puzzle::getSize() const
{
	return this->_size;
}

size_t Puzzle::getData() const
{
	return this->_data;
}

int Puzzle::operator[](int index) const
{
	if (index >= this->_size * this->_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	return (this->_data >> (index * 4)) & 0xF;
}

int Puzzle::getAt(int index) const
{
	if (index >= this->_size * this->_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	return (this->_data >> (index * 4)) & 0xF;
}

void Puzzle::setAt(int index, int value)
{
	if (index >= this->_size * this->_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	this->_data &= ~(0xFL << (index * 4));
	this->_data |= (static_cast<size_t>(value) << (index * 4));
}

int Puzzle::getAt(int y, int x) const
{
	if (y >= this->_size)
		throw std::out_of_range(catArgs(y, " is out of range"));
	if (x >= this->_size)
		throw std::out_of_range(catArgs(x, " is out of range"));
	return (this->_data >> ((y * this->_size + x) * 4)) & 0xF;
}

void Puzzle::setAt(int y, int x, int value)
{
	if (y >= this->_size)
		throw std::out_of_range(catArgs(y, " is out of range"));
	if (x >= this->_size)
		throw std::out_of_range(catArgs(x, " is out of range"));
	this->_data &= ~(0xFL << ((y * this->_size + x) * 4));
	this->_data |= (static_cast<size_t>(value) << ((y * this->_size + x) * 4));
}

int Puzzle::getAt(const Position &pos) const
{
	if (pos.y >= this->_size)
		throw std::out_of_range(catArgs(pos.y, " is out of range"));
	if (pos.x >= this->_size)
		throw std::out_of_range(catArgs(pos.x, " is out of range"));
	return (this->_data >> ((pos.y * this->_size + pos.x) * 4)) & 0xF;
}

void Puzzle::setAt(const Position &pos, int value)
{
	if (pos.y >= this->_size)
		throw std::out_of_range(catArgs(pos.y, " is out of range"));
	if (pos.x >= this->_size)
		throw std::out_of_range(catArgs(pos.x, " is out of range"));
	this->_data &= ~(0xFL << ((pos.y * this->_size + pos.x) * 4));
	this->_data |= (static_cast<size_t>(value) << ((pos.y * this->_size + pos.x) * 4));
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

void Puzzle::print(std::ostream &os, bool displaySize) const
{
	int width = std::to_string(this->_size * this->_size - 1).length();

	if (displaySize)
		os << this->_size << std::endl;
	for (int i = 0; i < this->_size; i++)
	{
		for (int j = 0; j < this->_size; j++)
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
	for (int y = 0; y < this->_size; y++)
	{
		for (int x = 0; x < this->_size; x++)
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

	if (newPos.y >= this->_size || newPos.y < 0 || newPos.x >= this->_size || newPos.x < 0)
		return {};

	Puzzle newPuzzle(*this);
	newPuzzle.swap(newPuzzle._emptyPos, newPos);
	newPuzzle._emptyPos = newPos;
	newPuzzle.updateParameters();
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

Puzzle Puzzle::getGoal(int size)
{
	Puzzle goal(size);
	int    dir = 0, nb = size, v = 1, x = -1, y = 0;

	for (; v != size * size; nb--)
	{
		for (int i = nb == size || nb == 1 ? 1 : 0; i < 2; i++)
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
				goal.setAt(y, x, v);
				v++;
			}
			dir = (dir + 1) % 4;
		}
	}
	return goal;
}

void Puzzle::updateParameters()
{
	this->setH();
}

int Puzzle::getH() const
{
	return this->_h;
}

void Puzzle::setH()
{
	this->_h = Puzzle::heuristicFunction ? Puzzle::heuristicFunction(*this) : 0;
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
	Puzzle::heuristicFunction = heuristicFunction;
}
