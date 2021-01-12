template<typename T>
Array<T>::Array(): _size(0), _data(nullptr)
{}

template<typename T>
Array<T>::Array(uint size): _size(size), _data(std::make_unique<T[]>(size))
{}

template<typename T>
Array<T>::Array(uint size, const T &value): _size(size), _data(std::make_unique<T[]>(size))
{
	for (uint i = 0; i < size; i++)
		_data[i] = value;
}

template<typename T>
Array<T>::Array(const Array &array)
{
	*this = array;
}

template<typename T>
Array<T>::Array(Array &&array)
{
	*this = std::move(array);
}

template<typename T>
Array<T> &Array<T>::operator=(const Array<T> &array)
{
	if (this != &array)
	{
		this->_size = array._size;
		this->_data.reset();
		this->_data = std::make_unique<T[]>(this->_size);
		for (uint i = 0; i < this->_size; i++)
			_data[i] = array[i];
	}
	return *this;
}

template<typename T>
Array<T> &Array<T>::operator=(Array<T> &&array)
{
	if (this != &array)
	{
		this->_data.reset();
		this->_size = array._size;
		this->_data = std::move(array._data);
	}
	return *this;
}

template<typename T>
Array<T>::~Array()
{}

template<typename T>
uint Array<T>::getSize() const
{
	return this->_size;
}

template<typename T>
const T &Array<T>::operator[](uint index) const
{
	return this->_data[index];
}

template<typename T>
T &Array<T>::operator[](uint index)
{
	return this->_data[index];
}
