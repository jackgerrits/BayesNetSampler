#ifndef BAYESNODE_H
#define BAYESNODE_H

#include <vector>
#include <string>
#include <utility>
#include <cassert>
#include <cstdlib>

#include "util.h"

class BayesNode {
public:
    std::vector<double> probabilities;    // Probability table stores the values for true, false can be calculated as (1-p).
    std::vector<bool> parents;
    int selfIndex;
    std::string name;

public:
    BayesNode(std::vector<double> probabilities, std::vector<bool> parents, int selfIndex, std::string name);

    std::pair<std::vector<int>, double> query(std::pair<std::vector<int>, double> evidence);
    int getIndex(std::vector<int> evidence);
    double getProbability(std::vector<int> evidence);
};

#endif
