
#include "huffman.h"

#include <set>

#include "../base/container_utils.h"
#include "../base/testing.h"

namespace huffman {

TEST(empty_huffman) {
  Huffman huff({}, "");
  ASSERT_TRUE(huff.is_successful());
}

TEST(simple_huffman) {
  Huffman huff({0.5, 0.1, 0.3, 0.4}, "5134");
  ASSERT_TRUE(huff.is_successful());
}

TEST(fail_create_huffman) {
  {
    Huffman huff({0.4}, "");
    ASSERT_FALSE(huff.is_successful());
  }
  {
    Huffman huff({0.4}, "45");
    ASSERT_FALSE(huff.is_successful());
  }
}

bool AllPrefixesAreUnique(const std::vector<std::string>& codes) {
  std::set<std::string> prefixes;
  for (const std::string& code : codes) {
    prefixes.insert(code);
  }

  for (const std::string& code : codes) {
    std::string prefix;
    for (char c : code) {
      prefix.push_back(c);
      if (prefix.size() == code.size()) continue;
      if (util::ContainsKey(prefixes, prefix)) {
        LOG(ERROR) << "Prefix exist as a code: \"" << prefix << "\"!";
        return false;
      }
    }
  }
  return true;
}

bool CodingIsConsistent(const std::vector<std::string>& codes,
                        const std::vector<float>& distr) {
  for (size_t i = 0; i < codes.size(); ++i) {
    for (size_t j = i + 1; j < codes.size(); ++j) {
      bool fail = false;
      if (distr[i] < distr[j] && codes[i].size() < codes[j].size()) {
        // j is more common but has longer code
        fail = true;
      }
      if (distr[i] > distr[j] && codes[i].size() > codes[j].size()) {
        // i is more common but has longer code
        fail = true;
      }
      if (fail) {
        LOG(ERROR) << "Inconsistent at " << i << " vs " << j
                   << ":\n" << distr[i] << " vs " << distr[j]
                   << "\n" << codes[i] << " vs " << codes[j];
        return false;
      }
    }
  }
  return true;
}

// Wrapper of new interface to old interface. Useful for legacy tests.
std::vector<std::string> HuffmanCodes(std::vector<float> distribution) {
  std::vector<char> tokens;
  char token = 'a';
  for (size_t i = 0; i < distribution.size(); ++i, ++token) {
    tokens.push_back(token);
  }

  Huffman huff(distribution, tokens);
  std::vector<std::string> result;
  for (char t : tokens) {
    result.push_back(huff.Code(t));
  }
  return result;
}

TEST(huffman_old_simple) {
  std::vector<float> distr = {0.5, 0.1, 0.3, 0.4};
  auto codes = HuffmanCodes(distr);
  ASSERT_EQ(distr.size(), codes.size());
  ASSERT_TRUE(AllPrefixesAreUnique(codes)) << codes;
  ASSERT_TRUE(CodingIsConsistent(codes, distr)) << codes << distr;
}

size_t IndexOfLetter(char c) {
  CHECK(c >= 'a') << c;
  CHECK(c <= 'z') << c;
  return c - 'a';
}

TEST(huffman_english) {
  auto distr = EnglishLetterDistribution();
  auto codes = HuffmanCodes(distr);
  ASSERT_EQ(26, codes.size());
  ASSERT_TRUE(AllPrefixesAreUnique(codes)) << codes;

  ASSERT_EQ(IndexOfLetter('e'), 4);
  ASSERT_GT(distr[IndexOfLetter('e')], distr[IndexOfLetter('s')]);
  ASSERT_LT(codes[IndexOfLetter('e')].size(), codes[IndexOfLetter('s')].size());

  ASSERT_TRUE(CodingIsConsistent(codes, distr)) << codes << distr;
  /*
  for (char c = 'a'; c <= 'z'; ++c) {
    size_t i = IndexOfLetter(c);
    LOG(INFO) << c << ": " << distr[i] << " " << codes[i];
  }
  */
}

}  // namespace huffman