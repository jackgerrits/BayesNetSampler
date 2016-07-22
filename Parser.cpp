#include "Parser.h"

std::pair<int, std::vector<int> > Parser::parseQuery(std::string query, std::vector<std::string> names) {
    std::regex rgx("(?:P\\()?(?:(\\w+)(?:\\s*\\=?\\s*)?(\\w+)?)+", std::regex::ECMAScript);
    std::smatch result;

    std::vector<int> observations(names.size(), 0); // Initialise default observations with values of zero
    int queryIndex;

    std::regex_search(query, result, rgx);
    // The regex returns the entire match and then the group afterwards.
    queryIndex = find(names.begin(), names.end(), result[1]) - names.begin();
    query = result.suffix().str();

    while (std::regex_search (query,result,rgx)) {
        int index = find(names.begin(), names.end(), result[1]) - names.begin();
        std::string boolStr = result[2];
        std::transform(boolStr.begin(), boolStr.end(), boolStr.begin(), ::tolower);
        observations[index] = boolStr == "true" ? 1 : -1;
        query = result.suffix().str();
    }

    return std::make_pair(queryIndex, observations);
}


std::vector<BayesNode> Parser::parseNetworkFile(std::ifstream& file){
    int numNodes;
    file >> numNodes;

    std::vector<std::string> names;
    for(int i = 0; i < numNodes; i++){
        std::string currentName;
        file >> currentName;
        names.push_back(currentName);
    }

    std::vector<std::vector<bool>> relationMatrix(numNodes, std::vector<bool>(numNodes, false));
    for(int i = 0; i < numNodes; ++i){
        for(int j = 0; j < numNodes; ++j){
            int currentRelation;
            file >> currentRelation;
            relationMatrix[i][j] = currentRelation == true;
        }
    }

    std::vector<std::vector<double>> nodeProbabilities(numNodes);
    std::vector<std::vector<bool>> relationTranspose(numNodes);

    for(int nodeIndex = 0; nodeIndex < numNodes; ++nodeIndex){
        int parents = 0;
        // determine number of parents to current node;
        for(int row = 0; row < numNodes; ++row){
            parents += relationMatrix[row][nodeIndex];
            relationTranspose[nodeIndex].push_back(relationMatrix[row][nodeIndex]);
        }

        for(int i = 0; i < (1 << parents); ++i){
            // negProb is ignored but must be extracted from the stream.
            double posProb, negProb;
            file >> posProb >> negProb;

            nodeProbabilities[nodeIndex].push_back(posProb);
        }
    }

    std::vector<BayesNode> nodes;
    for(int i = 0; i < numNodes; i++){
        nodes.push_back(BayesNode(nodeProbabilities[i], relationTranspose[i], i, names[i]));
    }

    return nodes;
}
