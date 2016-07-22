#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <utility>
#include <regex>
#include <iostream>
#include <string>
#include <fstream>

#include "BayesNode.h"

class Parser {
public:
    static std::pair<int, std::vector<int>> parseQuery(std::string query, std::vector<std::string> names);
    static std::vector<BayesNode> parseNetworkFile(std::ifstream& file);
};

#endif
