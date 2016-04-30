#ifndef DAG_H
#define DAG_H

#include <queue>
#include <unordered_map>
#include <set>
#include <vector>

#include "graph.h"
#include "../base/logging.h"

namespace graph {

// Complexity: O(E) = O(d*V)
// Implemented by topological sorting
template <typename NodeId>
bool IsDag(const Graph<NodeId>& graph);

template <typename NodeId>
bool TopologicalSorting(const Graph<NodeId>& graph, std::vector<NodeId>* out);

// Implementation -----------------------------

template <typename NodeId>
bool IsDag(const Graph<NodeId>& graph) {
  typename Graph<NodeId>::Path unused;
  return TopologicalSorting<NodeId>(graph, &unused);
}

template <typename NodeId>
bool TopologicalSorting(const Graph<NodeId>& graph, std::vector<NodeId>* out) {
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

  typename Graph<NodeId>::Path result;

  // Go through each Node in order of hierarchy: O(V)
  while (!queue.empty()) {
    NodeId node = queue.front();
    queue.pop();
    result.push_back(node);

    auto neighbours = graph.GetNeighbours(node);
    if (neighbours == nullptr) {
      LOG(ERROR) << "Should never happen!!!";
      return false;
    }
    for (NodeId neighbour : *neighbours) {
      incoming_edges[neighbour].erase(node);
      if (0 == incoming_edges[neighbour].size()) {
        queue.push(neighbour);
      }
    }
  }

  const bool is_dag = result.size() == graph.Nodes().size();
  if (is_dag) {
    out->swap(result);
  }
  return is_dag;
}

}  // namespace graph

#endif
