#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
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

template <typename T, typename F>
std::vector<T> mapTransform(std::vector<F> container, const std::function <T (F)>& func) {
    std::vector<T> vec;
    for(auto element : container) {
        vec.push_back(func(element));
    }
    return vec;
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
        mapTransform<string, BayesNode>(nodes, [] (BayesNode node) { return node.name; }));

    return 0;
}
