#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "gtest/gtest.h"
#include "BayesNode.h"
#include "Parser.h"

TEST(NodeTests, NodeIndexTest) {
    // Alarm node in Alarm network example.
    std::vector<double> probabilities = {0.005, 0.29, 0.94, 0.95};
    std::vector<bool> parents = {true, true, false, false, false};
    int selfIndex = 2;

    BayesNode bayesNode(probabilities, parents, selfIndex, "testNode");

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

    BayesNode bayesNode(probabilities, parents, selfIndex, "testNode");

    EXPECT_NEAR(bayesNode.getProbability({-1, -1, 1, 0, 0}), 0.005, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({-1, 1, 1, 0, 0}), 0.29, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({1, -1, 1, 0, 0}), 0.94, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({1, 1, 1, 0, 0}), 0.95, 0.00001);

    EXPECT_NEAR(bayesNode.getProbability({-1, -1, -1, 0, 0}), 0.995, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({-1, 1, -1, 0, 0}), 0.71, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({1, -1, -1, 0, 0}), 0.06, 0.00001);
    EXPECT_NEAR(bayesNode.getProbability({1, 1, -1, 0, 0}), 0.05, 0.00001);
}

TEST(NodeTests, NodeQueryPositiveTests) {
    srand(time(NULL));

    // Alarm node in Alarm network example.
    std::vector<double> probabilities = {0.005, 0.29, 0.94, 0.95};
    std::vector<bool> parents = {true, true, false, false, false};
    int selfIndex = 2;

    BayesNode bayesNode(probabilities, parents, selfIndex, "testNode");

    std::vector<int> observations = {-1, 1, 0, 0, 0};
    auto initialObservations = std::make_pair(observations, 1.0);

    int positiveCount = 0;
    const int TOTAL_RUNS = 1000;
    for(int i = 0; i < TOTAL_RUNS; ++i){
        auto result = bayesNode.queryWithWeight(initialObservations);
        positiveCount += result.first[selfIndex] == 1 ? 1 : 0;
    }

    double probResult = (double)positiveCount/(double)TOTAL_RUNS;

    // Given -burgular, +earthquake probability of +alarm is 0.29
    // Will assume it is at this value with some threshold.

    // Actual value is 0.29
    EXPECT_GT(probResult, 0.25);
    EXPECT_LT(probResult, 0.35);
}

TEST(NodeTests, NodeQueryNegativeTests) {
    srand(time(NULL));

    // Alarm node in Alarm network example.
    std::vector<double> probabilities = {0.005, 0.29, 0.94, 0.95};
    std::vector<bool> parents = {true, true, false, false, false};
    int selfIndex = 2;

    BayesNode bayesNode(probabilities, parents, selfIndex, "testNode");

    std::vector<int> observations = {-1, -1, 0, 0, 0};
    auto initialObservations = std::make_pair(observations, 1.0);

    int negativeCount = 0;
    const int TOTAL_RUNS = 1000;
    for(int i = 0; i < TOTAL_RUNS; ++i){
        auto result = bayesNode.queryWithWeight(initialObservations);
        negativeCount += result.first[selfIndex] == -1 ? 1 : 0;
    }

    double probResult = (double)negativeCount/(double)TOTAL_RUNS;

    // Given -burgular, +earthquake probability of +alarm is 0.29
    // Will assume it is at this value with some threshold.

    // Actaul value is 0.995
    EXPECT_GT(probResult, 0.9);
    EXPECT_LT(probResult, 1.0);
}

TEST(NodeTests, ParseQueryTest) {
    std::vector<std::string> names = {"Burglar", "Earthquake", "Alarm", "John", "Mary"};
    std::string query ="P(Burglar | John=true, Mary=true)";

    auto result = Parser::parseQuery(query, names);

    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, std::vector<int>({0, 0, 0, 1, 1}));
}

TEST(NodeTests, BurgularNetworkLikelihoodTest) {
    srand(time(NULL));

    std::ifstream file("burglarnetwork.txt");
    ASSERT_TRUE(file.is_open());

    std::vector<BayesNode> nodes = Parser::parseNetworkFile(file);

    std::string queryLine = "P(Burglar | John=true, Mary=true)";
    std::pair<int, std::vector<int>> query = Parser::parseQuery(queryLine,
        vectorMap<std::string, BayesNode>(nodes, [] (BayesNode node) { return node.name; }));

    const int NUM_SAMPLES = 100000;
    std::pair<double, double> result = likelihoodSampling(query, nodes, NUM_SAMPLES);

    // Actaul value is 0.452634
    EXPECT_GT(result.first, 0.40);
    EXPECT_LT(result.first, 0.50);

    // Actaul value is  0.547366
    EXPECT_GT(result.second, 0.50);
    EXPECT_LT(result.second, 0.60);
}

TEST(NodeTests, BurgularNetworkRejectionTest) {
    srand(time(NULL));

    std::ifstream file("burglarnetwork.txt");
    ASSERT_TRUE(file.is_open());

    std::vector<BayesNode> nodes = Parser::parseNetworkFile(file);

    std::string queryLine = "P(Burglar | John=true, Mary=true)";
    std::pair<int, std::vector<int>> query = Parser::parseQuery(queryLine,
        vectorMap<std::string, BayesNode>(nodes, [] (BayesNode node) { return node.name; }));

    const int NUM_SAMPLES = 100000;
    std::pair<double, double> result = rejectionSampling(query, nodes, NUM_SAMPLES);

    // Actaul value is 0.452634
    EXPECT_GT(result.first, 0.40);
    EXPECT_LT(result.first, 0.50);

    // Actaul value is  0.547366
    EXPECT_GT(result.second, 0.50);
    EXPECT_LT(result.second, 0.60);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

