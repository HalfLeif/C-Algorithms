
#include "tree.h"

#include "graph.h"
#include "../base/testing.h"

namespace graph {

TEST(empty_graph_not_tree) {
  auto graph = GraphBuilder<std::string>::UndirectedGraph().Build();
  ASSERT_FALSE(IsUndirectedTree(graph));
}

TEST(small_undirected_tree) {
  auto graph = GraphBuilder<std::string>::UndirectedGraph()
      .AddEdge("a", "b").AddEdge("a", "c").AddEdge("c", "d")
      .AddEdge("d", "e").AddEdge("c", "f").Build();
  ASSERT_TRUE(IsUndirectedTree(graph));
}

TEST(small_undirected_cycle) {
  auto graph = GraphBuilder<std::string>::UndirectedGraph()
      .AddEdge("a", "b").AddEdge("a", "c").AddEdge("c", "d")
      .AddEdge("d", "e").AddEdge("c", "f").AddEdge("b", "e").Build();
  ASSERT_FALSE(IsUndirectedTree(graph));
}

TEST(small_directed_tree) {
  auto graph = GraphBuilder<std::string>::DirectedGraph()
      .AddEdge("a", "b").AddEdge("a", "c").AddEdge("c", "d")
      .AddEdge("d", "e").AddEdge("c", "f").Build();
  ASSERT_TRUE(IsTree<std::string>(graph, "a"));
}

TEST(small_directed_merge) {
  auto graph = GraphBuilder<std::string>::DirectedGraph()
      .AddEdge("a", "b").AddEdge("a", "c").AddEdge("c", "d")
      .AddEdge("d", "e").AddEdge("c", "f").AddEdge("b", "e").Build();
  ASSERT_FALSE(IsTree<std::string>(graph, "a"));
}

}  // namespace graph
