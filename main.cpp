#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>
#include <cmath>

#include "BayesNode.h"
#include "Parser.h"
#include "util.h"

using namespace std;

// Sampling algorithms are located in BayesNode.h so that they could be referenced from test.cpp
// and not located in the main file.

int main(int argc, char const *argv[]) {
    bool useRejection = false;
    string networkFile;

    if (argc == 2){
        networkFile = argv[1];
    } else if (argc == 3){
        networkFile = argv[2];

        if(strcmp(argv[1], "-r") == 0){
            useRejection = true;
        } else {
            cerr << "Invalid option: " << argv[1] << endl;
            cerr << "USAGE: " << argv[0] << " [-r] networkDefinitionFile " << endl;
            exit(1);
        }
    } else {
        cerr << "USAGE: " << argv[0] << " [-r] networkDefinitionFile " << endl;
        exit(1);
    }

    ifstream file(networkFile);
    if (!file.is_open()){
        cerr << "Failed opening network file" << endl;
        exit(1);
    }

    // Seed the RNG with current time.
    srand(time(NULL));

    // Process the network file.
    vector<BayesNode> nodes = Parser::parseNetworkFile(file);
    file.close();

    // Read in a line and process it as the query.
    string queryLine;
    getline(cin, queryLine);
    pair<int, vector<int>> query = Parser::parseQuery(queryLine,
        vectorMap<string, BayesNode>(nodes, [] (BayesNode node) { return node.name; }));

    const int NUM_SAMPLES = 1000;
    pair<double, double> result;
    if(useRejection){
        result = rejectionSampling(query, nodes, NUM_SAMPLES);
    } else {
        result = likelihoodSampling(query, nodes, NUM_SAMPLES);
    }

    cout << result.first << ", " << result.second << endl;

    return 0;
}
