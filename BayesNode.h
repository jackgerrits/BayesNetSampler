#ifndef BAYESNODE_H
#define BAYESNODE_H

#include <vector>
#include <utility>
#include <cassert>
#include <cstdlib>

double randDouble();

class BayesNode {
protected:
    std::vector<double> probabilities;    // Probability table stores the values for true, false can be calculated.
    std::vector<bool> parents;
    int selfIndex;

public:
    BayesNode(std::vector<double> probabilities, std::vector<bool> parents, int selfIndex);

    std::pair<std::vector<int>, double> query(std::pair<std::vector<int>, double> evidence);
    int getIndex(std::vector<int> evidence);
    double getProbability(std::vector<int> evidence);

    // double getProbability(std::vector<int> observations);
};

#endif
