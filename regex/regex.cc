#include "regex.h"

namespace regex {

bool Regex::Matches(const std::string& input) const {
  internal::VisitedSet visited;
  return MatchesInternal(input.c_str(), 0, &visited);
}

bool Regex::MatchesInternal(const char* input, const int current_state,
                            internal::VisitedSet* visited) const {
  const std::pair<const char*, int> pair = std::make_pair(input, current_state);
  if (util::ContainsKey(*visited, pair)) {
    return false;
  }

  const char c = *input;
  const bool reached_end = c == '\0';
  if (current_state >= nodes_.size()) {
    return reached_end;
  } 
  const bool can_recur = nodes_[current_state].IsRecurrent();
  const bool matches = !reached_end && nodes_[current_state].Matches(c);
  
  // Consume node and char
  if (matches && !can_recur && MatchesInternal(input + 1, current_state + 1, visited)) {
    return true;
  }
  
  // Consume char, keep state
  if (matches && can_recur && MatchesInternal(input + 1, current_state, visited)) {
    return true;
  }
  
  // Consume node, keep char
  if (can_recur && MatchesInternal(input, current_state + 1, visited)) {
    return true;
  }
  visited->insert(std::move(pair));
  return false;
}

// static
Regex RegexBuilder::Build(const std::string& pattern, bool* failed) {
  RegexBuilder builder;
  for (char c : pattern) {
    builder.Add(c);
  }
  *failed = builder.failed_;
  return builder.out_;
}

void RegexBuilder::Add(char c) {
  if (failed_) {
    return;
  }
  if (c == '*') {
    if (out_.nodes_.empty() || out_.nodes_.back().IsRecurrent()) {
      failed_ = true;
    } else {
      out_.nodes_.back().MakeRecurrent();
    }
  } else {
    out_.nodes_.emplace_back(c);
  }
}
}  // namespace regex
