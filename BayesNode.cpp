#include "BayesNode.h"

BayesNode::BayesNode(std::vector<double> probabilities, std::vector<bool> parents, int selfIndex, std::string name)
    : probabilities(probabilities)
    , parents(parents)
    , selfIndex(selfIndex)
    , name(name)
{ }

std::pair<std::vector<int>, double> BayesNode::queryWithWeight(std::pair<std::vector<int>, double> evidence){
    // If the index of this node already has evidence it means that it was given as evidence
    // No need to sample, but must mutate weight.
    bool isSelfEvidence = evidence.first[selfIndex] != 0;

    if(isSelfEvidence){
        evidence.second = evidence.second * getProbability(evidence.first);
    } else {
        evidence.first = query(evidence.first);
    }

    return evidence;
}

std::vector<int> BayesNode::query(std::vector<int> evidence){
    // If this sample already contains a value for this node, just send it back.
    if(evidence[selfIndex] != 0){
        return evidence;
    }

    double selfProb = getProbability(evidence);
    double queryValue = randDouble();

    if(queryValue <= selfProb){ // Case is positive
        evidence[selfIndex] = 1;
    } else { // Case is negative
        evidence[selfIndex] = -1;
    }

    return evidence;
}

// Returns the probability of positive self case when self has no observation.
// If self has an observation, will return corresponding positive or negative case.
double BayesNode::getProbability(std::vector<int> evidence){
    int index = getIndex(evidence);

    // Queries the positive probability from cpt.
    double prob = probabilities[index];

    // If observation explicitly states negative observation, return that probability.
    if(evidence[selfIndex] == -1){
        prob = 1 - prob;
    }

    return prob;
}

int BayesNode::getIndex(std::vector<int> evidence){
    assert(evidence.size() == parents.size());

    int index = 0;
    int curVal = 1;
    for(int i = evidence.size() - 1; i >= 0; --i){
        if(parents[i]) { // Current index is a parent
            assert(evidence[i] != 0); // Each piece of evidence corresponding to a parent should be set

            if(evidence[i] == 1){   // If the current node is positive evidence, add the current binary value to the index
                index += curVal;
            }
            curVal = curVal << 1;   // Only increase the binary value when we are looking at parents
        }
    }

    return index;
}
