#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "BayesNode.h"
#include "Parser.h"
#include "util.h"

using namespace std;

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
    for(auto n : nodes){
        cout << n.name << endl;
    }

    // Read in a line and process it as the query.
    string queryLine;
    getline(cin, queryLine);
    pair<int, vector<int>> query = Parser::parseQuery(queryLine,
        vectorMap<string, BayesNode>(nodes, [] (BayesNode node) { return node.name; }));


    return 0;
}
