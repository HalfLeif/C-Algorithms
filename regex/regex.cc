#include "regex.h"

namespace regex {
bool Regex::MatchesInternal(const char* input, const int current_state) const {
  const char c = *input;
  const bool reached_end = c == '\0';
  if (current_state >= nodes_.size()) {
    return reached_end;
  } 
  const bool can_recur = nodes_[current_state].IsRecurrent();
  const bool matches = !reached_end && nodes_[current_state].Matches(c);
  
  // Consume node and char
  if (matches && !can_recur && MatchesInternal(input + 1, current_state + 1)) {
    return true;
  }
  
  // Consume char, keep state
  if (matches && can_recur && MatchesInternal(input + 1, current_state)) {
    return true;
  }
  
  // Consume node, keep char
  if (can_recur && MatchesInternal(input, current_state + 1)) {
    return true;
  }
  return false;
}

void RegexBuilder::Add(char c) {
  if (failed_) {
    return;
  }
  if (c == '*') {
    if (out_.nodes_.empty()) {
      failed_ = true;
    } else {
      out_.nodes_.back().MakeRecurrent();
    }
  } else {
    out_.nodes_.emplace_back(c);
  }
}
}  // namespace regex
