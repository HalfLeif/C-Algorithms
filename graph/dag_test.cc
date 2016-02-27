
#include "dag.h"

#include "graph.h"
#include "../base/testing.h"

namespace graph {
TEST(basic_dag) {
  auto graph =
      GraphBuilder<std::string>::DirectedGraph()
      .AddEdge("start", "a").AddEdge("start", "b").AddEdge("b", "c").Build();
  ASSERT_TRUE(IsDag(graph));
}

TEST(dag_cyclic) {
  auto graph =
      GraphBuilder<std::string>::DirectedGraph()
      .AddEdge("start", "a").AddEdge("a", "b").AddEdge("b", "c").AddEdge("c", "d").AddEdge("d", "b").Build();
  ASSERT_FALSE(IsDag(graph));
}

TEST(dag_disconnected) {
  auto graph =
      GraphBuilder<std::string>::DirectedGraph()
      .AddEdge("start", "a").AddEdge("island", "b").AddEdge("b", "c").Build();
  ASSERT_TRUE(IsDag(graph));
}

TEST(dag_tree) {
  auto graph =
      GraphBuilder<std::string>::DirectedGraph()
      .AddEdge("a", "root").AddEdge("b", "root").AddEdge("c", "b").Build();
  ASSERT_TRUE(IsDag(graph));
}

}  // namespace graph
