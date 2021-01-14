template<typename... Args>
std::string catArgs(const Args &...args)
{
	std::stringstream sstream;

	(sstream << ... << args);
	return sstream.str();
}

template<typename T>
void mySwap(T &a, T &b)
{
	T tmp;

	tmp = a;
	a = b;
	b = tmp;
}
