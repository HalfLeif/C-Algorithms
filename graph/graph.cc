
#include "graph.h"

namespace graph {
namespace {
template <typename Container, typename Key>
bool ContainsKey(const Container& container, const Key& key) {
  return container.find(key) != container.end();
}
}  // namespace

const Edge* Graph::GetEdge(const NodeId& from, const NodeId& to) const {
  std::pair<NodeId, NodeId> pair = std::make_pair(from, to);
  if (ContainsKey(edges_, pair)) {
    return &edges_.at(pair);
  }
  return nullptr;
}

const std::vector<NodeId>* Graph::GetNeighbours(const NodeId& id) const {
  if (ContainsKey(neighbours_, id)) {
    return &neighbours_.at(id);
  }
  return nullptr;
}

GraphBuilder& GraphBuilder::AddEdge(const NodeId& from, const NodeId& to, Edge e) {
  if (!g_.HasNode(from)) {
    g_.neighbours_[from] = {to};
  } else {
    g_.neighbours_[from].push_back(to);
  }

  if (!g_.HasNode(to)) {
    g_.neighbours_[to] = {};
  }
  g_.edges_[std::make_pair(from, to)] = e;

  if (!directed_) {
    g_.neighbours_[to].push_back(from);
    g_.edges_[std::make_pair(to, from)] = e;
  }
  return *this;
}
}  // namespace graph
