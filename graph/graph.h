// Representing graphs
//
// Example:
// auto builder = graph::GraphBuilder<const my::Node*>::DirectedGraph();
// auto graph = builder.AddEdge(a, b).AddEdge(b, c).Build();

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace graph {

// Can change this to a class with Annotations and stuff, leave for now
// Can also change to template argument or pointer type
typedef int Edge;

template <typename A, typename B>
struct PairHasher {
 public:
  size_t operator() (const std::pair<A, B>& pair) const {
    return hash_a_(pair.first) + 32503 * hash_b_(pair.second);
  }
 private:
  std::hash<A> hash_a_;
  std::hash<B> hash_b_;
};

template <typename NodeId>
class GraphBuilder;

template <typename NodeId = std::string>
class Graph {
 public:
  const Edge* GetEdge(const NodeId& from, const NodeId& to) const;
  const std::unordered_set<NodeId>* GetNeighbours(const NodeId& id) const;

  bool HasNode(const NodeId& id) const {
    return GetNeighbours(id) != nullptr;
  }

  bool HasEdge(const NodeId& from, const NodeId& to) const {
    return GetEdge(from, to) != nullptr;
  }

 private:
  Graph() {}

  std::unordered_map<NodeId, std::unordered_set<NodeId>> neighbours_;
  std::unordered_map<std::pair<NodeId, NodeId>, Edge, PairHasher<NodeId, NodeId>> edges_;

  friend class GraphBuilder<NodeId>;
};

template <typename NodeId = std::string>
class GraphBuilder {
 public:
  static GraphBuilder UndirectedGraph() {
    return GraphBuilder(false);
  }
  static GraphBuilder DirectedGraph() {
    return GraphBuilder(true);
  }

  GraphBuilder& AddEdge(const NodeId& from, const NodeId& to, Edge e);

  inline GraphBuilder& AddEdge(const NodeId& from, const NodeId& to) {
    return AddEdge(from, to, Edge());
  }
  Graph<NodeId> Build() { return std::move(g_); }

 private:
  GraphBuilder(bool directed) : directed_(directed) {}

  const bool directed_;
  Graph<NodeId> g_;
};

// Implementation ----------------------------------------
template <typename Container, typename Key>
bool ContainsKey(const Container& container, const Key& key) {
  return container.find(key) != container.end();
}

template <typename NodeId>
const Edge* Graph<NodeId>::GetEdge(const NodeId& from, const NodeId& to) const {
  std::pair<NodeId, NodeId> pair = std::make_pair(from, to);
  if (ContainsKey(edges_, pair)) {
    return &edges_.at(pair);
  }
  return nullptr;
}

template <typename NodeId>
const std::unordered_set<NodeId>* Graph<NodeId>::GetNeighbours(const NodeId& id) const {
  if (ContainsKey(neighbours_, id)) {
    return &neighbours_.at(id);
  }
  return nullptr;
}

template <typename NodeId>
GraphBuilder<NodeId>& GraphBuilder<NodeId>::AddEdge(const NodeId& from, const NodeId& to, Edge e) {
  if (!g_.HasNode(from)) {
    g_.neighbours_[from] = {to};
  } else {
    g_.neighbours_[from].insert(to);
  }

  if (!g_.HasNode(to)) {
    g_.neighbours_[to] = {};
  }
  g_.edges_[std::make_pair(from, to)] = e;

  if (!directed_) {
    g_.neighbours_[to].insert(from);
    g_.edges_[std::make_pair(to, from)] = e;
  }
  return *this;
}
}  // namespace graph
#endif
