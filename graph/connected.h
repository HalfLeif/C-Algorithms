#ifndef CONNECTED_H
#define CONNECTED_H

#include <queue>
#include <unordered_set>
#include <vector>

#include "graph.h"
#include "../base/container_utils.h"

namespace graph {

// Returns set of connected nodes (including itself)
// that are reachable by going `uphill`, `downhill` or both.
template <typename NodeId>
std::unordered_set<NodeId> SubGraph(const Graph<NodeId>& g, const NodeId& n, bool uphill, bool downhill);

template <typename NodeId>
std::unordered_set<NodeId> Connected(const Graph<NodeId>& g, const NodeId& n) {
  return SubGraph<NodeId>(g, n, true, true);
}

template <typename NodeId>
std::unordered_set<NodeId> Reachable(const Graph<NodeId>& g, const NodeId& n) {
  return SubGraph<NodeId>(g, n, false, true);
}

template <typename NodeId>
std::unordered_set<NodeId> Ancestors(const Graph<NodeId>& g, const NodeId& n) {
  return SubGraph<NodeId>(g, n, true, false);
}

// Returns disconnected subgraphs of the whole graph
// If is connected, returns the entire graph
template <typename NodeId>
std::vector<std::unordered_set<NodeId>> Partition(const Graph<NodeId>& g);

template <typename NodeId>
bool IsFullyConnected(const Graph<NodeId>& g) {
  if (g.Nodes().empty()) return true;
  const auto start = g.Nodes().begin()->first;
  return Connected(g, start).size() == g.Nodes().size();
}

// Implementation -----------------------------

template <typename NodeId>
std::unordered_set<NodeId> SubGraph(const Graph<NodeId>& g, const NodeId& start,
                                    const bool uphill, const bool downhill) {
  std::unordered_set<NodeId> connected = {start};
  std::queue<NodeId> queue;
  queue.push(start);

  while (!queue.empty()) {
    NodeId node = queue.front();
    queue.pop();
    connected.insert(node);
    if (downhill) {
      for (const NodeId& n : *g.GetNeighbours(node)) {
        if (util::ContainsKey(connected, n)) {
          continue;
        }
        queue.push(n);
      }
    }
    // Note: if is undirected, neighbours contains all uphill already
    if (uphill && g.IsDirected()) {
      for (const NodeId& n : *g.GetIncoming(node)) {
        if (util::ContainsKey(connected, n)) {
          continue;
        }
        queue.push(n);
      }
    }
  }
  return connected;
}

template <typename NodeId>
std::vector<std::unordered_set<NodeId>> Partition(const Graph<NodeId>& g) {
  std::vector<std::unordered_set<NodeId>> result;
  for (const auto& p : g.Nodes()){
    if (!util::ContainsInElem(result, p.first)) {
      std::unordered_set<NodeId> connected = Connected<NodeId>(g, p.first);
      result.push_back(std::move(connected));
    }
  }
  return result;
}



}  // namespace graph

#endif
