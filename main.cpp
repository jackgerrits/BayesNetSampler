#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <regex>
#include <algorithm>

using namespace std;

// Vector pipe overload to allow for easy printing and debugging
template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

std::pair<int, std::vector<int> > parseQuery(std::string query, std::vector<std::string> names) {
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
        string boolStr = result[2];
        std::transform(boolStr.begin(), boolStr.end(), boolStr.begin(), ::tolower);
        observations[index] = boolStr == "true" ? 1 : -1;
        query = result.suffix().str();
    }

    return std::make_pair(queryIndex, observations);
}

int main(int argc, char const *argv[]) {
    srand(time(NULL));  // Seed the RNG with current time.

    int numNodes;
    cin >> numNodes;

    cin.ignore(1000,'\n');

    std::vector<string> names;
    for(int i = 0; i < numNodes; i++){
        string currentName;
        cin >> currentName;
        names.push_back(currentName);
    }

    cin.ignore(1000,'\n');

    std::vector<string> parents;
    for(int i = 0; i < numNodes; i++){
        string currentName;
        getline(cin, currentName);
        parents.push_back(currentName);
    }

    cout << parents << endl;


    return 0;
}


// (?:P\()?(?:(\w+)(?:\=?)?(\w+)?)+

// 5
// Burglar Earthquake Alarm John Mary
// 00100
// 00100
// 00011
// 00000
// 00000

// 0.01 0.99

// 0.02 0.98

// 0.005 0.995
// 0.29 0.71
// 0.94 0.06
// 0.95 0.05

// 0.05 0.95
// 0.9 0.1

// 0.01 0.99
// 0.7 0.3
