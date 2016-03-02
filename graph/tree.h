#ifndef TREE_H
#define TREE_H

#include <unordered_set>

#include "graph.h"
#include "../base/container_utils.h"

namespace graph {
// Note: also works for Directed graphs
template <typename NodeId>
bool IsTree(const Graph<NodeId>& graph, NodeId root);

template <typename NodeId>
bool IsUndirectedTree(const Graph<NodeId>& graph) {
  const NodeId* first = graph.First();
  if (first == nullptr || graph.IsDirected()) return false;
  return IsTree(graph, *first);
}

// Implementation ----------------------------------------

namespace internal {
template <typename NodeId>
bool IsTreeInternal(const Graph<NodeId>& graph, const NodeId node,
                    const NodeId* parent, std::unordered_set<NodeId>* visited) {
  visited->insert(node);
  for (const NodeId& child : *graph.GetNeighbours(node)) {
    if (parent != nullptr && *parent == child) {
      continue;
    }
    if (util::ContainsKey(*visited, child)) {
      return false;
    }
    if (!IsTreeInternal(graph, child, &node, visited)) {
      return false;
    }
  }
  return true;
}
}  // namespace internal

template <typename NodeId>
bool IsTree(const Graph<NodeId>& graph, NodeId root) {
  std::unordered_set<NodeId> visited_set;
  return internal::IsTreeInternal<NodeId>(graph, root, nullptr, &visited_set);
}

}  // namespace graph

#endif
