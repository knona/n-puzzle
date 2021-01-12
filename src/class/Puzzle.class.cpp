#include "Puzzle.class.hpp"

#include "cat-args.template.hpp"

#include <cstring>
#include <exception>
#include <iomanip>

void Puzzle::init(uint size)
{
	this->_size = size;
	this->_data = std::make_unique<uint[]>(size * size);
	std::memset(this->_data.get(), 0, size * size);
}

Puzzle::Puzzle()
{
	this->_size = 0;
	this->_data = nullptr;
}

Puzzle::Puzzle(uint size)
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
		std::memcpy(this->_data.get(), puzzle._data.get(), this->_size * this->_size);
	}
	return *this;
}

Puzzle &Puzzle::operator=(Puzzle &&puzzle)
{
	if (this != &puzzle)
	{
		this->_data.reset();
		this->_size = puzzle._size;
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

std::ostream &operator<<(std::ostream &os, Puzzle &puz)
{
	puz.print(os);
	return os;
}
