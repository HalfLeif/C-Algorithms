
#include "regex.h"
#include "../base/testing.h"

namespace regex {

TEST(regex_empty) {
  bool failed;
  auto regex = RegexBuilder::Build("", &failed);
  ASSERT_FALSE(failed);
  ASSERT_TRUE(regex.Matches(""));
  ASSERT_FALSE(regex.Matches("a"));
}

TEST(regex_straight) {
  bool failed;
  auto regex = RegexBuilder::Build("abcdefaaa", &failed);
  ASSERT_FALSE(failed);
  ASSERT_TRUE(regex.Matches("abcdefaaa"));
  ASSERT_FALSE(regex.Matches("a"));
  ASSERT_FALSE(regex.Matches(""));
  ASSERT_FALSE(regex.Matches("abcdefaa"));
  ASSERT_FALSE(regex.Matches("abcdefaaaa"));
}

TEST(regex_dot) {
  bool failed;
  auto regex = RegexBuilder::Build("abc..fa.a", &failed);
  ASSERT_FALSE(failed);
  ASSERT_TRUE(regex.Matches("abcdefaaa"));
  ASSERT_FALSE(regex.Matches("a"));
  ASSERT_FALSE(regex.Matches(""));
  ASSERT_FALSE(regex.Matches("abcdefaa"));
  ASSERT_FALSE(regex.Matches("abcdefaaaa"));
}

TEST(regex_star) {
  bool failed;
  auto regex = RegexBuilder::Build("a*b*c", &failed);
  ASSERT_FALSE(failed);
  ASSERT_TRUE(regex.Matches("c"));
  ASSERT_TRUE(regex.Matches("aaac"));
  ASSERT_TRUE(regex.Matches("bbbc"));
  ASSERT_TRUE(regex.Matches("aabbbc"));
  ASSERT_FALSE(regex.Matches("a"));
  ASSERT_FALSE(regex.Matches("abac"));
  ASSERT_FALSE(regex.Matches("abfc"));
  ASSERT_FALSE(regex.Matches("aca"));
}

TEST(regex_dot_star) {
  bool failed;
  auto regex = RegexBuilder::Build(".*c", &failed);
  ASSERT_FALSE(failed);
  ASSERT_TRUE(regex.Matches("c"));
  ASSERT_TRUE(regex.Matches("bxfbc"));
  ASSERT_TRUE(regex.Matches("aabbbc"));
  ASSERT_FALSE(regex.Matches("a"));
  ASSERT_FALSE(regex.Matches(""));
  ASSERT_FALSE(regex.Matches("aca"));
}

// This use case shows why we need Dynamic programming,
// since we have a branch factor of 2 for each recurrent node
// for each char in the input.
TEST(regex_expanding) {
  bool failed;
  auto regex = RegexBuilder::Build(".*.*.*.*.*.*.*.*.*.*.*.*.*.*d", &failed);
  ASSERT_FALSE(failed);
  ASSERT_FALSE(regex.Matches("aabbbcasiduuiqgdkaushdmvquwyfdahgsdvmnbasvdqwfjudfahgsdvmbvqkywd_"));
}

TEST(regex_build_failure) {
  {
    bool failed = false;
    auto regex = RegexBuilder::Build("*", &failed);
    ASSERT_TRUE(failed);
  }
  {
    bool failed = false;
    auto regex = RegexBuilder::Build("a**", &failed);
    ASSERT_TRUE(failed);
  }
}

}  // namespace regex
