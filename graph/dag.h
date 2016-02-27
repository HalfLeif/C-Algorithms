#ifndef DAG_H
#define DAG_H

#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "graph.h"
#include "../base/logging.h"

namespace graph {

template <typename NodeId>
bool IsDag(const Graph<NodeId>& graph);

// Implementation -----------------------------

template <typename NodeId>
bool IsDag(const Graph<NodeId>& graph) {
  // Register all edges
  std::unordered_map<NodeId, int> incoming_edges;
  for (auto p : graph.Nodes()) {
    for (const NodeId& neighbour : p.second) {
      ++incoming_edges[neighbour];
    }
  }

  // Register starting positions
  std::queue<NodeId> queue;
  for (auto p : graph.Nodes()) {
    if (!ContainsKey(incoming_edges, p.first)) {
      queue.push(p.first);
    }
  }

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
      if (0 == --incoming_edges[neighbour]) {
        queue.push(neighbour);
      }
    }
  }

  return nodes_in_dag == graph.Nodes().size();
}

}  // namespace graph

#endif
