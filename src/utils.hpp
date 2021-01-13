#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>

template<typename... Args>
std::string catArgs(const Args &...args);

template<typename T>
void swap(T &a, T &b);

#include "utils.tpp"

#endif // !UTILS_HPP
