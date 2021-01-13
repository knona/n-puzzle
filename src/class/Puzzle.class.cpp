#include "Puzzle.class.hpp"

#include "utils.hpp"

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

uint *Puzzle::operator[](uint index)
{
	return this->_data.get() + this->_size * index;
}

const uint *Puzzle::operator[](uint index) const
{
	return this->_data.get() + this->_size * index;
}

uint &Puzzle::at(uint a, uint b)
{
	if (a >= this->_size)
		throw std::out_of_range(catArgs(a, " is out of range"));
	if (b >= this->_size)
		throw std::out_of_range(catArgs(b, " is out of range"));
	return this->_data[a * this->_size + b];
}

uint Puzzle::at(uint a, uint b) const
{
	if (a >= this->_size)
		throw std::out_of_range(catArgs(a, " is out of range"));
	if (b >= this->_size)
		throw std::out_of_range(catArgs(b, " is out of range"));
	return this->_data[a * this->_size + b];
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

void Puzzle::print(std::ostream &os) const
{
	uint width = std::to_string(this->_size * this->_size - 1).length();

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
	swap(this->at(this->_emptyPos), this->at(newPos));
	this->_emptyPos = newPos;
}
