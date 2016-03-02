// Representing graphs
//
// Works for both weighted, unweighted graphs as well as both directed and undirected graphs.
//
// Example:
// auto builder = graph::GraphBuilder<const my::Node*>::DirectedGraph();
// auto graph = builder.AddEdge(a, b).AddEdge(b, c).Build();

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../base/container_utils.h"

namespace graph {

// Can change this to a class with Annotations and stuff, leave for now
// Can also change to template argument or pointer type
typedef int Edge;

template <typename NodeId>
class GraphBuilder;

// All Get/Has-operations are in average O(1).
template <typename NodeId = std::string>
class Graph {
 public:
  typedef std::vector<NodeId> Path;

  bool IsDirected() const { return directed_; }

  const Edge* GetEdge(const NodeId& from, const NodeId& to) const;
  const std::unordered_set<NodeId>* GetNeighbours(const NodeId& id) const;
  const std::unordered_set<NodeId>* GetIncoming(const NodeId& id) const;

  // Outgoing edges
  const std::unordered_map<NodeId, std::unordered_set<NodeId>>& Nodes() const { return neighbours_; }

  // Incoming edges
  const std::unordered_map<NodeId, std::unordered_set<NodeId>>& Incoming() const {
    if (directed_) {
      return incoming_;
    }
    return neighbours_;
  }

  bool HasNode(const NodeId& id) const {
    return GetNeighbours(id) != nullptr;
  }

  bool HasEdge(const NodeId& from, const NodeId& to) const {
    return GetEdge(from, to) != nullptr;
  }

  // Returns any single node in the graph, or nullptr if the graph is empty
  const NodeId* First() const {
    if (neighbours_.empty()) {
      return nullptr;
    }
    return &neighbours_.begin()->first;
  }

 private:
  Graph(bool directed) : directed_(directed) {}

  const bool directed_;
  std::unordered_map<NodeId, std::unordered_set<NodeId>> neighbours_;
  std::unordered_map<NodeId, std::unordered_set<NodeId>> incoming_;
  std::unordered_map<std::pair<NodeId, NodeId>, Edge, util::PairHasher<NodeId, NodeId>> edges_;

  friend class GraphBuilder<NodeId>;
};

// Building the graph: O(E)
template <typename NodeId = std::string>
class GraphBuilder {
 public:
  static GraphBuilder UndirectedGraph() {
    return GraphBuilder(false);
  }
  static GraphBuilder DirectedGraph() {
    return GraphBuilder(true);
  }

  // O(1)
  GraphBuilder& AddEdge(const NodeId& from, const NodeId& to, Edge e);

  inline GraphBuilder& AddEdge(const NodeId& from, const NodeId& to) {
    return AddEdge(from, to, Edge(1));
  }
  Graph<NodeId> Build() { return std::move(g_); }

 private:
  GraphBuilder(bool directed) : g_(directed) {}

  Graph<NodeId> g_;
};

// Implementation ----------------------------------------
template <typename NodeId>
const Edge* Graph<NodeId>::GetEdge(const NodeId& from, const NodeId& to) const {
  std::pair<NodeId, NodeId> pair = std::make_pair(from, to);
  if (util::ContainsKey(edges_, pair)) {
    return &edges_.at(pair);
  }
  return nullptr;
}

template <typename NodeId>
const std::unordered_set<NodeId>* Graph<NodeId>::GetNeighbours(const NodeId& id) const {
  if (util::ContainsKey(neighbours_, id)) {
    return &neighbours_.at(id);
  }
  return nullptr;
}

template <typename NodeId>
const std::unordered_set<NodeId>* Graph<NodeId>::GetIncoming(const NodeId& id) const {
  if (util::ContainsKey(incoming_, id)) {
    return &incoming_.at(id);
  }
  return nullptr;
}

template <typename NodeId>
GraphBuilder<NodeId>& GraphBuilder<NodeId>::AddEdge(const NodeId& from, const NodeId& to, Edge e) {
  g_.neighbours_[from].insert(to);
  g_.neighbours_[to];

  g_.edges_[std::make_pair(from, to)] = e;

  if (g_.IsDirected()) {
    g_.incoming_[to].insert(from);
    g_.incoming_[from];
  } else {
    g_.neighbours_[to].insert(from);
    g_.edges_[std::make_pair(to, from)] = e;
  }
  return *this;
}
}  // namespace graph
#endif
