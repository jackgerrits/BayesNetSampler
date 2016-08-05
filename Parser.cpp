#include "Parser.h"

// Searches for the given element in the given container.
// Returns the index of the element or -1 if not found.s
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
        std::transform(currentName.begin(), currentName.end(), currentName.begin(), ::tolower);
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
    // This regex simply matches words and these are interpretted in the intended sequence.
    std::regex rgx("\\w+", std::regex::ECMAScript);
    std::smatch result;

    // Initialise default observations with values of zero.
    std::vector<int> observations(names.size(), 0);

    // Extract P out of query.
    if(!std::regex_search(query, result, rgx)){
        std::cerr << "Error there was no query given." << std::endl;
        exit(1);
    }
    query = result.suffix().str();

    // Extract query variable.
    std::regex_search(query, result, rgx);
    std::string queryName = result[0];
    std::transform(queryName.begin(), queryName.end(), queryName.begin(), ::tolower);
    int queryIndex = findIndex(names, queryName);
    if(queryIndex == -1){
        std::cerr << queryName
            << " was not found in the network defintion, perhaps check spelling." << std::endl;
        exit(1);
    }

    query = result.suffix().str();

    // Keep searching while there are still evidence pairs.
    while(std::regex_search(query, result, rgx)){
        // Extract evidence variable name.
        std::string currentName = result[0];
        std::transform(currentName.begin(), currentName.end(), currentName.begin(), ::tolower);
        int index = findIndex(names, currentName);
        if(index == -1){
            std::cerr << currentName
                << " was not found in the network defintion, perhaps check spelling." << std::endl;
            exit(1);
        }
        query = result.suffix().str();

        // Attempt to extract the boolean, transform to lower case just in case and then convert into a negative or positive observation.
        if(std::regex_search(query, result, rgx)){
            std::string boolStr = result[0];
            std::transform(boolStr.begin(), boolStr.end(), boolStr.begin(), ::tolower);
            observations[index] = boolStr == "true" ? 1 : -1;
            query = result.suffix().str();
        } else {
            std::cerr << "Missing boolean value of evidence variable or query is malformed." << std::endl;
            exit(1);
        }
    }

    return std::make_pair(queryIndex, observations);
}
