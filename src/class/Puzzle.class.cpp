#include "Puzzle.class.hpp"

#include "utils.hpp"

#include <boost/functional/hash.hpp>
#include <cstdio>
#include <exception>
#include <iomanip>

void Puzzle::init(uint size)
{
	this->_size = size;
	this->_data = std::make_unique<uint[]>(size * size);
	for (uint i = 0; i < this->_size * this->_size; i++)
		this->_data[i] = 0;
}

Puzzle::Puzzle(): _size(0), _data(nullptr), _isEmptyPosDefined(false)
{}

Puzzle::Puzzle(uint size): _isEmptyPosDefined(false)
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
		for (uint i = 0; i < this->_size * this->_size; i++)
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
		this->_data = std::move(puzzle._data);
	}
	return *this;
}

Puzzle::~Puzzle()
{}

uint Puzzle::getSize() const
{
	return this->_size;
}

uint &Puzzle::operator[](uint index)
{
	if (index >= this->_size * this->_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	return this->_data[index];
}

uint Puzzle::operator[](uint index) const
{
	if (index >= this->_size * this->_size)
		throw std::out_of_range(catArgs(index, " is out of range"));
	return this->_data[index];
}

uint &Puzzle::at(uint y, uint x)
{
	if (y >= this->_size)
		throw std::out_of_range(catArgs(y, " is out of range"));
	if (x >= this->_size)
		throw std::out_of_range(catArgs(x, " is out of range"));
	return this->_data[y * this->_size + x];
}

uint Puzzle::at(uint y, uint x) const
{
	if (y >= this->_size)
		throw std::out_of_range(catArgs(y, " is out of range"));
	if (x >= this->_size)
		throw std::out_of_range(catArgs(x, " is out of range"));
	return this->_data[y * this->_size + x];
}

uint &Puzzle::at(Position pos)
{
	if (pos.x >= this->_size)
		throw std::out_of_range(catArgs(pos.x, " is out of range"));
	if (pos.y >= this->_size)
		throw std::out_of_range(catArgs(pos.y, " is out of range"));
	return this->_data[pos.y * this->_size + pos.x];
}

uint Puzzle::at(Position pos) const
{
	if (pos.x >= this->_size)
		throw std::out_of_range(catArgs(pos.x, " is out of range"));
	if (pos.y >= this->_size)
		throw std::out_of_range(catArgs(pos.y, " is out of range"));
	return this->_data[pos.y * this->_size + pos.x];
}

void Puzzle::print(std::ostream &os, bool displaySize) const
{
	uint width = std::to_string(this->_size * this->_size - 1).length();

	if (displaySize)
		os << this->_size << std::endl;
	for (uint i = 0; i < this->_size; i++)
	{
		for (uint j = 0; j < this->_size; j++)
		{
			if (j != 0)
				os << " ";
			os << std::setw(width) << this->at(i, j);
		}
		os << std::endl;
	}
}

void Puzzle::setZeroPosition(const Position &pos)
{
	this->_emptyPos = pos;
	this->_isEmptyPosDefined = true;
}

void Puzzle::setZeroPosition()
{
	for (uint i = 0; i < this->_size * this->_size; i++)
	{
		if (this->_data[i] == 0)
		{
			this->_emptyPos.y = i / this->_size;
			this->_emptyPos.x = i % this->_size;
			this->_isEmptyPosDefined = true;
			break;
		}
	}
	throw std::logic_error("0 value not found to define empty position");
}

std::ostream &operator<<(std::ostream &os, const Puzzle &puz)
{
	puz.print(os);
	return os;
}

void Puzzle::move(Move direction)
{
	if (!this->_isEmptyPosDefined)
		throw std::logic_error("Cannot move, empty position is not set");

	Position newPos = this->_emptyPos;

	if (direction == Move::Top)
	{
		newPos.y++;
		if (newPos.y == this->_size)
			throw std::logic_error(catArgs("Cannot move top from position:\n", this->_emptyPos));
	}
	else if (direction == Move::Right)
	{
		newPos.x--;
		if (newPos.x == std::numeric_limits<uint>::max())
			throw std::logic_error(catArgs("Cannot move right from position:\n", this->_emptyPos));
	}
	else if (direction == Move::Bottom)
	{
		newPos.y--;
		if (newPos.y == std::numeric_limits<uint>::max())
			throw std::logic_error(catArgs("Cannot move bottom from position:\n", this->_emptyPos));
	}
	else if (direction == Move::Left)
	{
		newPos.x++;
		if (newPos.x == this->_size)
			throw std::logic_error(catArgs("Cannot move left from position:\n", this->_emptyPos));
	}
	mySwap(this->at(this->_emptyPos), this->at(newPos));
	this->_emptyPos = newPos;
}

std::optional<Puzzle> Puzzle::moveAndRet(Move direction) const
{
	if (!this->_isEmptyPosDefined)
		throw std::logic_error("Cannot move, empty position is not set");

	Position newPos = this->_emptyPos;

	if (direction == Move::Top)
	{
		newPos.y++;
		if (newPos.y == this->_size)
			return {};
	}
	else if (direction == Move::Right)
	{
		newPos.x--;
		if (newPos.x == std::numeric_limits<uint>::max())
			return {};
	}
	else if (direction == Move::Bottom)
	{
		newPos.y--;
		if (newPos.y == std::numeric_limits<uint>::max())
			return {};
	}
	else if (direction == Move::Left)
	{
		newPos.x++;
		if (newPos.x == this->_size)
			return {};
	}
	Puzzle newPuzzle(*this);
	mySwap(newPuzzle.at(newPuzzle._emptyPos), newPuzzle.at(newPos));
	newPuzzle._emptyPos = newPos;
	return newPuzzle;
}

bool Puzzle::operator==(const Puzzle &puzzle) const
{
	for (uint i = 0; i < this->_size * this->_size; i++)
		if (this->_data[i] != puzzle._data[i])
			return false;
	return true;
}
bool Puzzle::operator!=(const Puzzle &puzzle) const
{
	return !(*this == puzzle);
}

// works only for size <= 4
size_t Puzzle::HashFunction::operator()(const Puzzle &puzzle) const
{
	size_t seed = 0;

	for (uint i = 0; i < puzzle._size * puzzle._size; i++)
		seed |= static_cast<size_t>(puzzle[i]) << (4 * i);

	return seed;
}

Puzzle Puzzle::getFinalState(uint size)
{
	Puzzle ret(size);

	uint dir = 0;
	uint nb = size;
	uint v = 1;

	int x = -1, y = 0;
	while (v != size * size)
	{
		for (uint i = nb == size || nb == 1 ? 1 : 0; i < 2; i++)
		{
			for (uint j = 0; j < nb; j++)
			{
				if (dir == 0)
					x++;
				if (dir == 1)
					y++;
				if (dir == 2)
					x--;
				if (dir == 3)
					y--;
				ret.at(y, x) = v;
				v++;
			}
			dir = (dir + 1) % 4;
		}
		nb--;
	}

	return ret;
}

std::vector<Puzzle> Puzzle::getChildren() const
{
	std::vector<Puzzle> vec;

	for (uint i = 0; i < 4; i++)
	{
		try
		{
			std::optional<Puzzle> puzzle = this->moveAndRet(static_cast<Move>(i));
			if (puzzle.has_value())
				vec.push_back(puzzle.value());
		}
		catch (const std::exception &e)
		{}
	}
	return vec;
}
