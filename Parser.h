#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <utility>
#include <regex>
#include <string>
#include <cassert>
#include <fstream>
#include <iostream>

#include "BayesNode.h"
#include "util.h"

class Parser {
public:
    static std::vector<BayesNode> parseNetworkFile(std::ifstream& file);
    static std::pair<int, std::vector<int>> parseQuery(std::string query, std::vector<std::string> names);
};

#endif
