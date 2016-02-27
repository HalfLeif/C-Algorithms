
#include "graph.h"

#include "../base/testing.h"

namespace {
TEST(directed_graph_test) {
  graph::GraphBuilder builder = graph::GraphBuilder::DirectedGraph();
  const graph::Graph g = builder.AddEdge("a", "b").Build();

  ASSERT_TRUE(g.HasNode("a"));
  ASSERT_TRUE(g.HasNode("b"));
  ASSERT_FALSE(g.HasNode("x"));

  ASSERT_TRUE(g.HasEdge("a", "b"));
  ASSERT_FALSE(g.HasEdge("b", "a"));
  ASSERT_FALSE(g.HasEdge("a", "x"));
}

TEST(undirected_graph_test) {
  graph::GraphBuilder builder = graph::GraphBuilder::UndirectedGraph();
  const graph::Graph g = builder.AddEdge("a", "b").Build();

  ASSERT_TRUE(g.HasNode("a"));
  ASSERT_TRUE(g.HasNode("b"));
  ASSERT_FALSE(g.HasNode("x"));

  ASSERT_TRUE(g.HasEdge("a", "b"));
  ASSERT_TRUE(g.HasEdge("b", "a"));
  ASSERT_FALSE(g.HasEdge("a", "x"));
}

TEST(weighted_graph_test) {
  graph::GraphBuilder builder = graph::GraphBuilder::UndirectedGraph();
  const graph::Graph g = builder.AddEdge("a", "b", 5).Build();

  ASSERT_TRUE(g.HasNode("a"));
  ASSERT_TRUE(g.HasNode("b"));
  ASSERT_FALSE(g.HasNode("x"));

  ASSERT_TRUE(g.HasEdge("a", "b"));
  ASSERT_TRUE(g.HasEdge("b", "a"));
  ASSERT_FALSE(g.HasEdge("a", "x"));

  ASSERT_EQ(5, *g.GetEdge("a", "b"));
  ASSERT_EQ(5, *g.GetEdge("b", "a"));
}

TEST(neighbour_graph_test) {
  graph::GraphBuilder builder = graph::GraphBuilder::UndirectedGraph();
  const graph::Graph g = builder.AddEdge("a", "b").AddEdge("b", "c").AddEdge("d", "b").Build();

  const std::vector<std::string>* neighbours = g.GetNeighbours("b");
  std::vector<std::string> expect = {"a", "c", "d"};
  ASSERT_TRUE(neighbours != nullptr);
  if (neighbours != nullptr) {
    ASSERT_TRUE(expect == *neighbours);
  }
}
}  // namespace
