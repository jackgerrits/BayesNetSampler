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

    std::pair<std::vector<int>, double> queryWithWeight(std::pair<std::vector<int>, double> evidence);
    std::vector<int> query(std::vector<int> evidence);
    int getIndex(std::vector<int> evidence);
    double getProbability(std::vector<int> evidence);
};

std::pair<double, double> likelihoodSampling(std::pair<int, std::vector<int>> query, std::vector<BayesNode> network, int numSamples);
std::pair<double, double> rejectionSampling(std::pair<int, std::vector<int>> query, std::vector<BayesNode> network, int numSamples);

#endif
