#include "Parser.h"

// Searches for the given element in the given container.
// Returns the index of the element or -1 if not found.
template <typename T>
int findIndex(std::vector<T> container, T searchElem){
    auto findIterator = find(container.begin(), container.end(), searchElem);
    if(findIterator == container.end()){
        return -1;
    }
    return findIterator - container.begin();
}

std::vector<BayesNode> Parser::parseNetworkFile(std::ifstream& file){
    int numNodes;
    file >> numNodes;

    // Read in numNodes number of names from the input file.
    std::vector<std::string> names;
    for(int i = 0; i < numNodes; i++){
        std::string currentName;
        file >> currentName;
        names.push_back(currentName);
    }

    // Read in the relation matrix into a 2d vector.
    std::vector<std::vector<bool>> relationMatrix(numNodes, std::vector<bool>(numNodes, false));
    for(int i = 0; i < numNodes; ++i){
        for(int j = 0; j < numNodes; ++j){
            int currentRelation;
            file >> currentRelation;
            relationMatrix[i][j] = currentRelation == true;
        }
    }

    std::vector<std::vector<double>> nodeProbabilities(numNodes);
    // Transpose is required as nodes need a list of parents/dependancies (column) rather than the children (row).
    std::vector<std::vector<bool>> relationTranspose(numNodes);

    // Iterate over the number of nodes, reading each of their probability tables.
    for(int nodeIndex = 0; nodeIndex < numNodes; ++nodeIndex){
        int parents = 0;
        // Determine number of parents for current node by going down column.
        // Also add these values into the transpose matrix.
        for(int row = 0; row < numNodes; ++row){
            parents += relationMatrix[row][nodeIndex];
            relationTranspose[nodeIndex].push_back(relationMatrix[row][nodeIndex]);
        }

        // Then read the next 2^parents lines as the probability table.
        for(int i = 0; i < (1 << parents); ++i){
            // negProb is ignored but must be extracted from the stream.
            double posProb, negProb;
            file >> posProb >> negProb;

            nodeProbabilities[nodeIndex].push_back(posProb);
        }
    }

    // Construct the list of BayesNodes to return as the network.
    std::vector<BayesNode> nodes;
    for(int i = 0; i < numNodes; i++){
        nodes.push_back(BayesNode(nodeProbabilities[i], relationTranspose[i], i, names[i]));
    }

    return nodes;
}

std::pair<int, std::vector<int> > Parser::parseQuery(std::string query, std::vector<std::string> names) {
    // This regex extracts the query parameters and then each evidence pair as subgroups.
    std::regex rgx("(?:P\\()?(?:(\\w+)(?:\\s*\\=?\\s*)?(\\w+)?)+", std::regex::ECMAScript);
    std::smatch result;

    // Initialise default observations with values of zero
    std::vector<int> observations(names.size(), 0);

    // Initial regex_search for the query variable.
    std::regex_search(query, result, rgx);
    // The regex returns the entire match and then the group afterwards, so the first element is ignored.
    int queryIndex = findIndex(names, std::string(result[1]));
    if(queryIndex == -1){
        std::cerr << result[1] << " was not found in the network defintion. Check spelling or capitalisation." << std::endl;
        exit(1);
    }

    query = result.suffix().str();

    // Iterate over each of the successive evidence pair matches adding them onto the observation vector.
    while (std::regex_search (query,result,rgx)) {
        // Determines the index of the given random variable name in the node list
        int index = findIndex(names, std::string(result[1]));
        if(index == -1){
            std::cerr << result[1] << " was not found in the network defintion. Check spelling or capitalisation." << std::endl;
            exit(1);
        }
        // Extract the boolean from the subgroup, transform to lower case just in case and then convert into a negative or positive observation.
        std::string boolStr = result[2];
        std::transform(boolStr.begin(), boolStr.end(), boolStr.begin(), ::tolower);
        observations[index] = boolStr == "true" ? 1 : -1;
        query = result.suffix().str();
    }

    return std::make_pair(queryIndex, observations);
}
