#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <vector>

namespace graph {

// Can change this to a class with Annotations and stuff, leave for now
// Can also change to template argument or pointer type
typedef int Edge;
typedef std::string NodeId;

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

class GraphBuilder;

class Graph {
 public:
  const Edge* GetEdge(const NodeId& from, const NodeId& to) const;
  const std::vector<NodeId>* GetNeighbours(const NodeId& id) const;

  bool HasNode(const NodeId& id) const {
    return GetNeighbours(id) != nullptr;
  }

  bool HasEdge(const NodeId& from, const NodeId& to) const {
    return GetEdge(from, to) != nullptr;
  }

 private:
  Graph() {}

  std::unordered_map<NodeId, std::vector<NodeId>> neighbours_;
  std::unordered_map<std::pair<NodeId, NodeId>, Edge, PairHasher<NodeId, NodeId>> edges_;

  friend class GraphBuilder;
};

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
  Graph Build() { return std::move(g_); }

 private:
  GraphBuilder(bool directed) : directed_(directed) {}

  const bool directed_;
  Graph g_;
};

}  // namespace graph
#endif
