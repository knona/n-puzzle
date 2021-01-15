#include "Puzzle.class.hpp"

#include "utils.hpp"

#include <cstdio>
#include <exception>
#include <iomanip>
#include <utility>

int (*Puzzle::heuristicFunction)(const Puzzle &) = nullptr;

void Puzzle::init(int size)
{
	this->_size = size;
	this->_data = std::make_unique<int[]>(size * size);
	for (int i = 0; i < this->_size * this->_size; i++)
		this->_data[i] = 0;
}

Puzzle::Puzzle(): _size(0), _data(nullptr), _isEmptyPosDefined(false), _g(std::numeric_limits<int>::max())
{}

Puzzle::Puzzle(int size): _isEmptyPosDefined(false), _g(std::numeric_limits<int>::max())
{
	this->init(size);
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
		this->_data.reset();
		init(puzzle._size);
		this->_emptyPos = puzzle._emptyPos;
		this->_isEmptyPosDefined = puzzle._isEmptyPosDefined;
		this->_h = puzzle._h;
		this->_g = puzzle._g;
		this->_hash = puzzle._hash;
		for (int i = 0; i < this->_size * this->_size; i++)
			this->_data[i] = puzzle._data[i];
	}
	return *this;
}

Puzzle &Puzzle::operator=(Puzzle &&puzzle)
{
	if (this != &puzzle)
	{
		this->_data.reset();
		this->_size = puzzle._size;
		this->_emptyPos = puzzle._emptyPos;
		this->_isEmptyPosDefined = puzzle._isEmptyPosDefined;
		this->_h = puzzle._h;
		this->_g = puzzle._g;
		this->_hash = puzzle._hash;
		this->_data = std::move(puzzle._data);
	}
	return *this;
}

Puzzle::~Puzzle()
{}

int Puzzle::getSize() const
{
	return this->_size;
}

int &Puzzle::operator[](int index)
{
	if (index >= this->_size * this->_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	return this->_data[index];
}

int Puzzle::operator[](int index) const
{
	if (index >= this->_size * this->_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	return this->_data[index];
}

int &Puzzle::at(int index)
{
	if (index >= this->_size * this->_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	return this->_data[index];
}

int Puzzle::at(int index) const
{
	if (index >= this->_size * this->_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	return this->_data[index];
}

int &Puzzle::at(int y, int x)
{
	if (y >= this->_size)
		throw std::out_of_range(catArgs(y, " is out of range"));
	if (x >= this->_size)
		throw std::out_of_range(catArgs(x, " is out of range"));
	return this->_data[y * this->_size + x];
}

int Puzzle::at(int y, int x) const
{
	if (y >= this->_size)
		throw std::out_of_range(catArgs(y, " is out of range"));
	if (x >= this->_size)
		throw std::out_of_range(catArgs(x, " is out of range"));
	return this->_data[y * this->_size + x];
}

int &Puzzle::at(Position pos)
{
	if (pos.x >= this->_size)
		throw std::out_of_range(catArgs(pos.x, " is out of range"));
	if (pos.y >= this->_size)
		throw std::out_of_range(catArgs(pos.y, " is out of range"));
	return this->_data[pos.y * this->_size + pos.x];
}

int Puzzle::at(Position pos) const
{
	if (pos.x >= this->_size)
		throw std::out_of_range(catArgs(pos.x, " is out of range"));
	if (pos.y >= this->_size)
		throw std::out_of_range(catArgs(pos.y, " is out of range"));
	return this->_data[pos.y * this->_size + pos.x];
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
			os << std::setw(width) << this->at(i, j);
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
	this->_isEmptyPosDefined = true;
}

void Puzzle::setZeroPosition()
{
	for (int i = 0; i < this->_size * this->_size; i++)
	{
		if ((*this)[i] == 0)
		{
			this->_emptyPos.y = i / this->_size;
			this->_emptyPos.x = i % this->_size;
			this->_isEmptyPosDefined = true;
			break;
		}
	}
	throw std::logic_error("0 value not found to define empty position");
}

bool Puzzle::operator==(const Puzzle &puzzle) const
{
	for (int i = 0; i < this->_size * this->_size; i++)
		if ((*this)[i] != puzzle[i])
			return false;
	return true;
}
bool Puzzle::operator!=(const Puzzle &puzzle) const
{
	return !(*this == puzzle);
}

std::optional<Puzzle> Puzzle::move(Move direction) const
{
	if (!this->_isEmptyPosDefined)
		throw std::logic_error("Cannot move, empty position is not set");

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
	std::swap(newPuzzle.at(newPuzzle._emptyPos), newPuzzle.at(newPos));
	newPuzzle._emptyPos = newPos;
	newPuzzle.updateParameters();
	return newPuzzle;
}

std::vector<Puzzle> Puzzle::getChildren() const
{
	std::vector<Puzzle> children;

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
				goal.at(y, x) = v;
				v++;
			}
			dir = (dir + 1) % 4;
		}
	}
	goal._hash = goal.hash();
	return goal;
}

// works only for size <= 4
size_t Puzzle::hash() const
{
	size_t seed = 0;

	for (int i = 0; i < this->_size * this->_size; i++)
		seed |= static_cast<size_t>((*this)[i]) << (4 * i);

	return seed;
}

void Puzzle::updateParameters()
{
	this->_h = Puzzle::heuristicFunction(*this);
	this->_hash = this->hash();
}

size_t Puzzle::getHash() const
{
	return this->_hash;
}

int Puzzle::getH() const
{
	return this->_h;
}

void Puzzle::setG(int value)
{
	this->_g = value;
}

int Puzzle::getG() const
{
	return this->_g;
}

int Puzzle::getF() const
{
	return this->_g + this->_h;
}

void Puzzle::setHeuristicFunction(int (*heuristicFunction)(const Puzzle &))
{
	Puzzle::heuristicFunction = heuristicFunction;
}
