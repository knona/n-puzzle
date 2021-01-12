#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <memory>
#include <utility>

typedef unsigned int uint;

template<typename T>
class Array
{
	private:
	uint                 _size;
	std::unique_ptr<T[]> _data;

	public:
	Array();
	Array(uint size);
	Array(uint size, const T &value);
	Array(const Array<T> &);
	Array<T> &operator=(const Array<T> &);
	Array(Array<T> &&);
	Array<T> &operator=(Array<T> &&);
	~Array();

	uint     getSize() const;
	const T &operator[](uint index) const;
	T &      operator[](uint index);
};

#include "Array.class.tpp"

#endif // !ARRAY_HPP
