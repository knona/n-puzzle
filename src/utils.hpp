#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>

template<typename... Args>
std::string catArgs(const Args &...args);

#include "utils.tpp"

#endif // !UTILS_HPP
