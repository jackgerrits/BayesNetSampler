#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <functional>

#include "BayesNode.h"
#include "Parser.h"

using namespace std;

template<typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
    out << "{";
    for(int i = 0; i < v.size(); i++) {
        out << v[i];
        if(i != (v.size()-1)) {
            out << ", ";
        }
    }
    out << "}";
    return out;
}

// Applies a mapping operation over each element of the vector and returns
// another vector holding the result of these operations.
// Accepts a vector holding abritrary values and a lambda function that accepts
// the given vectors type and returns an abritrary type.
template <typename S, typename T>
std::vector<S> vectorMap(const std::vector<T>& container, const std::function <S (T)>& func) {
    // Create a new vector of the return type.
    std::vector<S> resultVector;
    // Iterate over each element in the provided vector and push the result of of using the given func onto the result vector.
    for (T element : container) {
        resultVector.push_back(func(element));
    }
    return resultVector;
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

    srand(time(NULL));  // Seed the RNG with current time.

    // Process the network file
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
