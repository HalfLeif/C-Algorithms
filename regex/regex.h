// Simple Regex
//
// Currently only supports . and * operators, without escaping.
//
// TODO: increase supported regex features and partial matching
#ifndef REGEX_H
#define REGEX_H

#include <string>
#include <unordered_set>
#include <vector>

#include "../base/container_utils.h"


namespace regex {
namespace internal {
class Node {
 public:
  explicit Node(char allowed) : allowed_(allowed) {}
  
  bool Matches(char c) const {
    return allowed_ == '.' || allowed_ == c;
  }
  bool IsRecurrent() const { return recurrent_; }
  void MakeRecurrent() { recurrent_ = true; }
 private:
  const char allowed_;
  bool recurrent_ = false;
};

typedef std::unordered_set<std::pair<const char*, int>,
                           util::PairHasher<const char*, int>> VisitedSet;

}  // namespace internal

class RegexBuilder;

class Regex {
 public:
  // Full match
  bool Matches(const std::string& input) const;

 private:
  Regex() {};

  // Currently without DP:
  bool MatchesInternal(const char* input, const int current_state,
                       internal::VisitedSet* visited) const;

  std::vector<internal::Node> nodes_;
  
  friend class RegexBuilder;
};

class RegexBuilder {
 public:
  static Regex Build(const std::string& pattern, bool* failed);

 private:
  RegexBuilder() {}
  void Add(char c);

  Regex out_;
  bool failed_ = false;
};

}  // namespace regex

#endif
