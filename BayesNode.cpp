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

    // If there is already an observation for this node itself, then modify the weight of this sample.
    // Otherwise, query for this node setting a value.
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

// Implements the likelihood sampling algorithm, accepts a query (queryIndex, initialObservations), the network and the number of samples to take
std::pair<double, double> likelihoodSampling(std::pair<int, std::vector<int>> query, std::vector<BayesNode> network, int numSamples) {
    double positiveWeight = 0.0;
    double negativeWeight = 0.0;

    for(int i = 0; i < numSamples; ++i){
        // Create the intial observations for this sample, using query initialObservations
        std::pair<std::vector<int>, double> observations = std::make_pair(std::vector<int>(query.second), 1.0);

        // Iterate over each node in the network, querying it to modify the observations.
        for (size_t nodeIndex = 0; nodeIndex < network.size(); ++nodeIndex) {
            observations = network[nodeIndex].queryWithWeight(observations);
        }

        // If query value is positive.
        if(observations.first[query.first] == 1){
            positiveWeight += observations.second;
        } else { // Query value is negative.
            negativeWeight += observations.second;
        }
    }

    // Determine the positive and negative probabilities according to weights.
    return std::make_pair(positiveWeight / (positiveWeight + negativeWeight),
        negativeWeight / (positiveWeight + negativeWeight));
}

// Implements the rejection sampling algorithm, accepts a query (queryIndex, initialObservations), the network and the number of samples to take
std::pair<double, double> rejectionSampling(std::pair<int, std::vector<int>> query, std::vector<BayesNode> network, int numSamples) {
    int positiveCount = 0;
    int negativeCount = 0;

    for(int i = 0; i < numSamples; ++i){
        // Create initial observation as each random variable being unobserved.
        std::vector<int> observations = std::vector<int>(query.second.size(), 0);
        bool evidenceMismatch = false;

        // Iterate over each node in the network, querying to modify observations.
        for (size_t nodeIndex = 0; nodeIndex < network.size(); ++nodeIndex) {
            observations = network[nodeIndex].query(observations);

            // If there is a value set for the evidence at this node and the
            // observation doesn't match up with the required value.
            if(query.second[nodeIndex] != 0 && query.second[nodeIndex] != observations[nodeIndex]){
                // Set the variable to denote we are throwing this sample away.
                evidenceMismatch = true;
                break;
            }
        }

        // If there is an evidence mismatch, don't want to influence counts.
        if(evidenceMismatch){
            continue;
        }

        // If query value is positive.
        if(observations[query.first] == 1){
            positiveCount++;
        } else { // Query value is negative.
            negativeCount++;
        }
    }

    // Determine positive and negative probabilities from the counts that weren't rejected.
    return std::make_pair(positiveCount / (double)(positiveCount + negativeCount),
        negativeCount / (double)(positiveCount + negativeCount));
}
