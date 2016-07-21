#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <utility>
#include <regex>
#include <iostream>
#include <string>

class Parser {
public:
    static std::pair<int, std::vector<int> > parseQuery(std::string query, std::vector<std::string> names);
};

#endif
