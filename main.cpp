#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>

#include "BayesNode.h"
#include "Parser.h"
#include "util.h"

using namespace std;

pair<double, double> likelihoodSampling(pair<int, vector<int>> query, vector<BayesNode> nodes, int numSamples) {
    double positiveWeight = 0.0;
    double negativeWeight = 0.0;

    for(int i = 0; i < numSamples; ++i){
        pair<vector<int>, double> observations = make_pair(vector<int>(query.second), 1.0);

        for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex) {
            observations = nodes[nodeIndex].queryWithWeight(observations);
        }

        // If query value is positive.
        if(observations.first[query.first] == 1){
            positiveWeight += observations.second;
        } else { // Query value is negative.
            negativeWeight += observations.second;
        }
    }

    return make_pair(positiveWeight / (positiveWeight + negativeWeight), negativeWeight / (positiveWeight + negativeWeight));
}

pair<double, double> rejectionSampling(pair<int, vector<int>> query, vector<BayesNode> nodes, int numSamples) {
    int positiveCount = 0;
    int negativeCount = 0;

    for(int i = 0; i < numSamples; ++i){
        vector<int> observations = vector<int>(query.second.size(), 0);
        bool evidenceMismatch = false;

        for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex) {
            observations = nodes[nodeIndex].query(observations);

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

    cout << positiveCount << endl;
    cout << negativeCount << endl;


    return make_pair(positiveCount / (double)(positiveCount + negativeCount),
        negativeCount / (double)(positiveCount + negativeCount));
}


int main(int argc, char const *argv[]) {
    if(argc != 2){
        cerr << "USAGE: " << argv[0] << " networkDefinitonFile " << endl;
        exit(1);
    }

    ifstream file(argv[1]);
    if (!file.is_open()){
        cerr << "Failed opening network file" << endl;
        exit(1);
    }

    // Seed the RNG with current time.
    srand(time(NULL));

    // Process the network file.
    vector<BayesNode> nodes = Parser::parseNetworkFile(file);

    // Read in a line and process it as the query.
    string queryLine;
    getline(cin, queryLine);
    pair<int, vector<int>> query = Parser::parseQuery(queryLine,
        vectorMap<string, BayesNode>(nodes, [] (BayesNode node) { return node.name; }));

    pair<double, double> result = rejectionSampling(query, nodes, 1000);
    cout << result.first << ", " << result.second << endl;

    return 0;
}
