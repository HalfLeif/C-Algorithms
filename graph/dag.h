#ifndef DAG_H
#define DAG_H

#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "graph.h"
#include "../base/logging.h"

namespace graph {

// Complexity: O(E) = O(d*V)
// Implemented by topological sorting
template <typename NodeId>
bool IsDag(const Graph<NodeId>& graph);

// Implementation -----------------------------

template <typename NodeId>
bool IsDag(const Graph<NodeId>& graph) {
  if (!graph.IsDirected()) {
    return false;
  }
  auto incoming_edges = graph.Incoming();

  // Register starting positions: O(V)
  std::queue<NodeId> queue;
  for (auto p : incoming_edges) {
    if (p.second.size() == 0) {
      queue.push(p.first);
    }
  }

  // Go through each Node in order of hierarchy: O(V)
  int nodes_in_dag = 0;
  while (!queue.empty()) {
    NodeId node = queue.front();
    queue.pop();
    ++nodes_in_dag;

    auto neighbours = graph.GetNeighbours(node);
    if (neighbours == nullptr) {
      LOG(ERROR) << "Should never happen!!!";
    }
    for (NodeId neighbour : *neighbours) {
      incoming_edges[neighbour].erase(node);
      if (0 == incoming_edges[neighbour].size()) {
        queue.push(neighbour);
      }
    }
  }

  return nodes_in_dag == graph.Nodes().size();
}

}  // namespace graph

#endif
