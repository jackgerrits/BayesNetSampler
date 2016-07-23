#ifndef BAYES_UTIL_H
#define BAYES_UTIL_H

#include <cstdlib>
#include <vector>
#include <functional>
#include <iostream>
#include <string>

double randDouble();

template<typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v);

template <typename S, typename T>
std::vector<S> vectorMap(const std::vector<T>& container, const std::function <S (T)>& func);

#include "util.inl"

#endif
