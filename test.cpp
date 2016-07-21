#include <vector>
#include <cstdlib>
#include <ctime>

#include "gtest/gtest.h"
#include "BayesNode.h"
#include "Parser.h"

TEST(NodeTests, NodeIndexTest) {
    // Alarm node in Alarm network example.
    std::vector<double> probabilities = {0.005, 0.29, 0.94, 0.95};
    std::vector<bool> parents = {true, true, false, false, false};
    int selfIndex = 2;

    BayesNode bayesNode(probabilities, parents, selfIndex);

    EXPECT_EQ(bayesNode.getIndex({-1, -1, 0, 0, 0}), 0);
    EXPECT_EQ(bayesNode.getIndex({-1, 1, 0, 0, 0}), 1);
    EXPECT_EQ(bayesNode.getIndex({1, -1, 0, 0, 0}), 2);
    EXPECT_EQ(bayesNode.getIndex({1, 1, 0, 0, 0}), 3);
}

TEST(NodeTests, NodeWeightTests) {
    // Alarm node in Alarm network example.
    std::vector<double> probabilities = {0.005, 0.29, 0.94, 0.95};
    std::vector<bool> parents = {true, true, false, false, false};
    int selfIndex = 2;

    BayesNode bayesNode(probabilities, parents, selfIndex);

    EXPECT_NEAR(bayesNode.getProbability({-1, -1, 1, 0, 0}), 0.005, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({-1, 1, 1, 0, 0}), 0.29, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({1, -1, 1, 0, 0}), 0.94, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({1, 1, 1, 0, 0}), 0.95, 0.00001);

    EXPECT_NEAR(bayesNode.getProbability({-1, -1, -1, 0, 0}), 0.995, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({-1, 1, -1, 0, 0}), 0.71, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({1, -1, -1, 0, 0}), 0.06, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({1, 1, -1, 0, 0}), 0.05, 0.00001);
}


TEST(NodeTests, NodeQueryTests) {
    srand(time(NULL));

    // Alarm node in Alarm network example.
    std::vector<double> probabilities = {0.005, 0.29, 0.94, 0.95};
    std::vector<bool> parents = {true, true, false, false, false};
    int selfIndex = 2;

    BayesNode bayesNode(probabilities, parents, selfIndex);

    std::vector<int> observations = {-1, 1, 0, 0, 0};
    auto initialObservations = std::make_pair(observations, 1.0);

    int positiveCount = 0;
    const int TOTAL_RUNS = 100;
    for(int i = 0; i < TOTAL_RUNS; ++i){
        auto result = bayesNode.query(initialObservations);
        positiveCount += result.first[selfIndex] == 1 ? 1 : 0;
    }

    double probResult = (double)positiveCount/(double)TOTAL_RUNS;

    // Given -burgular, +earthquake probability of +alarm is 0.29
    // Will assume it is at this value with some threshold.

    EXPECT_GT(probResult, 0.25);
    EXPECT_LT(probResult, 0.35);
}

    static std::pair<int, std::vector<int> > parseQuery(std::string query, std::vector<std::string> names);


TEST(NodeTests, ParseQueryTest) {
    std::vector<std::string> names = {"Burglar", "Earthquake", "Alarm", "John", "Mary"};
    std::string query ="P(Burglar | John=true, Mary=true)";

    auto result = Parser::parseQuery(query, names);

    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, std::vector<int>({0, 0, 0, 1, 1}));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// int BayesNode::getIndex(std::vector<int> evidence){
//     BayesNode(std::vector<double> probabilities, std::vector<bool> parents, selfIndex);
