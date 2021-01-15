template<typename... Args>
std::string catArgs(const Args &...args)
{
	std::stringstream sstream;

	(sstream << ... << args);
	return sstream.str();
}
