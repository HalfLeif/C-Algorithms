
#include "graph.h"

#include "../base/testing.h"

namespace {
TEST(directed_graph_test) {
  graph::GraphBuilder<std::string> builder = graph::GraphBuilder<std::string>::DirectedGraph();
  const graph::Graph<std::string> g = builder.AddEdge("a", "b").Build();

  ASSERT_TRUE(g.IsDirected());

  ASSERT_TRUE(g.HasNode("a"));
  ASSERT_TRUE(g.HasNode("b"));
  ASSERT_FALSE(g.HasNode("x"));

  ASSERT_TRUE(g.HasEdge("a", "b"));
  ASSERT_FALSE(g.HasEdge("b", "a"));
  ASSERT_FALSE(g.HasEdge("a", "x"));
}

TEST(undirected_graph_test) {
  graph::GraphBuilder<std::string> builder = graph::GraphBuilder<std::string>::UndirectedGraph();
  const graph::Graph<std::string> g = builder.AddEdge("a", "b").Build();

  ASSERT_FALSE(g.IsDirected());

  ASSERT_TRUE(g.HasNode("a"));
  ASSERT_TRUE(g.HasNode("b"));
  ASSERT_FALSE(g.HasNode("x"));

  ASSERT_TRUE(g.HasEdge("a", "b"));
  ASSERT_TRUE(g.HasEdge("b", "a"));
  ASSERT_FALSE(g.HasEdge("a", "x"));
}

TEST(weighted_graph_test) {
  graph::GraphBuilder<std::string> builder = graph::GraphBuilder<std::string>::UndirectedGraph();
  const graph::Graph<std::string> g = builder.AddEdge("a", "b", 5).Build();

  ASSERT_TRUE(g.HasNode("a"));
  ASSERT_TRUE(g.HasNode("b"));
  ASSERT_FALSE(g.HasNode("x"));

  ASSERT_TRUE(g.HasEdge("a", "b"));
  ASSERT_TRUE(g.HasEdge("b", "a"));
  ASSERT_FALSE(g.HasEdge("a", "x"));

  ASSERT_EQ(5, *g.GetEdgeWeight("a", "b"));
  ASSERT_EQ(5, *g.GetEdgeWeight("b", "a"));
}

TEST(remove_directed_graph_test) {
  graph::GraphBuilder<std::string> builder = graph::GraphBuilder<std::string>::DirectedGraph();
  graph::Graph<std::string> g = builder.AddEdge("a", "a").AddEdge("a", "b").AddEdge("a", "c").AddEdge("b", "a").AddEdge("f", "a").Build();

  ASSERT_TRUE(g.HasEdge("a", "a"));
  ASSERT_TRUE(g.HasEdge("a", "b"));
  ASSERT_TRUE(g.HasEdge("b", "a"));
  ASSERT_TRUE(g.HasEdge("f", "a"));
  ASSERT_FALSE(g.HasEdge("a", "f"));

  ASSERT_TRUE(g.RemoveEdge("a", "b"));
  ASSERT_FALSE(g.HasEdge("a", "b"));
  ASSERT_TRUE(g.HasEdge("a", "a"));
  ASSERT_TRUE(g.HasEdge("b", "a"));
  ASSERT_TRUE(nullptr == g.GetEdgeWeight("a", "b"));
  ASSERT_FALSE(g.RemoveEdge("a", "b"));  // Cannot remove it again

  ASSERT_TRUE(g.HasEdge("a", "a"));
  ASSERT_TRUE(g.RemoveEdge("a", "a"));
  ASSERT_FALSE(g.HasEdge("a", "a"));
  ASSERT_TRUE(nullptr == g.GetEdgeWeight("a", "a"));
  ASSERT_FALSE(g.RemoveEdge("a", "a"));

  const std::set<std::string> expect_neighbours_a = {"c"};
  ASSERT_EQ(expect_neighbours_a, *g.GetNeighbours("a"));
  ASSERT_TRUE(g.GetIncoming("b")->empty());
}

TEST(remove_undirected_graph_test) {
  graph::GraphBuilder<std::string> builder = graph::GraphBuilder<std::string>::UndirectedGraph();
  graph::Graph<std::string> g = builder.AddEdge("a", "a").AddEdge("a", "b").AddEdge("a", "c").Build();

  ASSERT_TRUE(g.HasEdge("a", "a"));
  ASSERT_TRUE(g.HasEdge("a", "b"));
  ASSERT_TRUE(g.HasEdge("b", "a"));

  ASSERT_TRUE(g.RemoveEdge("a", "b"));
  ASSERT_FALSE(g.HasEdge("a", "b"));
  ASSERT_FALSE(g.HasEdge("b", "a"));
  ASSERT_TRUE(g.HasEdge("a", "a"));
  ASSERT_TRUE(nullptr == g.GetEdgeWeight("a", "b"));
  ASSERT_TRUE(nullptr == g.GetEdgeWeight("b", "a"));
  ASSERT_FALSE(g.RemoveEdge("a", "b"));  // Cannot remove it again
  ASSERT_FALSE(g.RemoveEdge("b", "a"));  // Cannot remove it again

  const std::set<std::string> expect_neighbours_a = {"a", "c"};
  auto* neighbours_a = g.GetNeighbours("a");
  ASSERT_EQ(expect_neighbours_a, *g.GetNeighbours("a"));
  ASSERT_TRUE(g.GetNeighbours("b")->empty());
  ASSERT_TRUE(g.GetIncoming("b")->empty());
}

TEST(neighbour_graph_test) {
  graph::GraphBuilder<std::string> builder = graph::GraphBuilder<std::string>::UndirectedGraph();
  const graph::Graph<std::string> g = builder.AddEdge("a", "b").AddEdge("b", "c").AddEdge("d", "b").Build();

  const std::set<std::string>* neighbours = g.GetNeighbours("b");
  std::set<std::string> expect = {"a", "c", "d"};
  ASSERT_TRUE(neighbours != nullptr);
  if (neighbours != nullptr) {
    ASSERT_EQ(expect, *neighbours);
  }
}

TEST(graph_template_test) {
  auto builder = graph::GraphBuilder<int>::DirectedGraph();
  auto graph = builder.AddEdge(0, 1).AddEdge(1, 5).Build();
  ASSERT_TRUE(graph.HasNode(0));
  ASSERT_TRUE(graph.HasNode(1));

  ASSERT_TRUE(graph.HasEdge(1, 5));
  ASSERT_FALSE(graph.HasEdge(5, 1));
}

struct TestNode{};

TEST(graph_template_ptr_test) {
  TestNode a;
  TestNode b;
  TestNode c;

  auto builder = graph::GraphBuilder<const TestNode*>::DirectedGraph();
  auto graph = builder.AddEdge(&a, &b).AddEdge(&b, &c).Build();
  ASSERT_TRUE(graph.HasNode(&a));
  ASSERT_TRUE(graph.HasNode(&b));

  ASSERT_TRUE(graph.HasEdge(&a, &b));
  ASSERT_TRUE(graph.HasEdge(&b, &c));
  ASSERT_FALSE(graph.HasEdge(&a, &c));
}

TEST(graph_abuse_test) {
  auto graph = graph::GraphBuilder<int>::UndirectedGraph().AddEdge(0, 0).AddEdge(0, 0).AddEdge(0, 0).Build();
  auto* neighbours = graph.GetNeighbours(0);
  ASSERT_TRUE(neighbours != nullptr);
  if (neighbours != nullptr) {
    ASSERT_EQ(1, neighbours->size());
  }
}
}  // namespace
