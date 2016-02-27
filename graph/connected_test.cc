#include "connected.h"

#include <string>
#include <unordered_set>

#include "../base/testing.h"

namespace graph {

TEST(reachable_test) {
  auto graph =
      GraphBuilder<std::string>::DirectedGraph()
      .AddEdge("a", "b").AddEdge("d", "b").AddEdge("b", "c")
      .AddEdge("a", "e").AddEdge("c", "a").AddEdge("x", "y").Build();
  auto reachable = Reachable<std::string>(graph, "b");
  std::unordered_set<std::string> expected = {"b", "c", "a", "e"};
  ASSERT_TRUE(reachable == expected);
}

TEST(ancestors_test) {
  auto graph =
      GraphBuilder<std::string>::DirectedGraph()
      .AddEdge("a", "b").AddEdge("d", "b").AddEdge("b", "c")
      .AddEdge("a", "e").AddEdge("c", "a").AddEdge("x", "y").Build();
  auto reachable = Ancestors<std::string>(graph, "b");
  std::unordered_set<std::string> expected = {"b", "c", "a", "d"};
  ASSERT_TRUE(reachable == expected);
}

TEST(partition_test) {
  auto graph =
      GraphBuilder<std::string>::DirectedGraph()
      .AddEdge("a", "b").AddEdge("d", "b").AddEdge("b", "c")
      .AddEdge("a", "e").AddEdge("c", "a").AddEdge("x", "y").Build();
  auto subgraphs = Partition<std::string>(graph);
  ASSERT_EQ(2, subgraphs.size());

  std::unordered_set<std::string> expected_big = {"b", "c", "a", "d", "e"};
  std::unordered_set<std::string> expected_small = {"x", "y"};
  ASSERT_TRUE(subgraphs[0] == expected_small);
  ASSERT_TRUE(subgraphs[1] == expected_big);
}

TEST(connected_test) {
  auto graph =
      GraphBuilder<std::string>::DirectedGraph()
      .AddEdge("a", "b").AddEdge("d", "b").AddEdge("b", "c")
      .AddEdge("a", "e").AddEdge("c", "a").AddEdge("x", "y").Build();

  ASSERT_FALSE(IsFullyConnected(graph));

  auto graph_2 =
      GraphBuilder<std::string>::DirectedGraph()
      .AddEdge("a", "b").AddEdge("d", "b").AddEdge("b", "c")
      .AddEdge("a", "e").AddEdge("c", "a").Build();

  ASSERT_TRUE(IsFullyConnected(graph_2));
}

}  // namespace graph
