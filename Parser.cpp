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
