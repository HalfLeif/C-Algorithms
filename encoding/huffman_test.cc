
#include "huffman.h"

#include <set>

#include "../base/container_utils.h"
#include "../base/testing.h"

namespace huffman {

TEST(empty_huffman) {
  Huffman huff({}, "");
  ASSERT_TRUE(huff.is_successful());

  // The escape node is also the root... Not necessarily a bad thing...
  // It means that the encoding operation will just be identity.
  ASSERT_EMPTY(huff.EscapeCode());
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

TEST(huffman_debug_codes) {
  Huffman huff({0.5, 0.1, 0.3, 0.4}, "abcd");
  const std::string escape = huff.EscapeCode();
  ASSERT_EQ(escape, huff.Code('g'));

  const std::string encoded = huff.Encoded('g');
  ASSERT_EQ('g', 0x67);  // == 0110 0111
  ASSERT_EQ(encoded, escape + "0110" + "0111");

  // For non-escaped chars, encoded is just the code itself:
  ASSERT_EQ(huff.Code('a'), huff.Encoded('a'));
}

bool AllPrefixesAreUnique(const std::vector<std::string>& codes) {
  std::set<std::string> prefixes;
  for (const std::string& code : codes) {
    prefixes.insert(code);
  }

  for (const std::string& code : codes) {
    std::string prefix;
    for (unsigned char c : code) {
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
  std::vector<unsigned char> tokens;
  unsigned char token = 'a';
  for (size_t i = 0; i < distribution.size(); ++i, ++token) {
    tokens.push_back(token);
  }

  Huffman huff(distribution, tokens);
  std::vector<std::string> result;
  for (unsigned char t : tokens) {
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

size_t IndexOfLetter(unsigned char c) {
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

  // For debug:
  /*
  std::vector<std::pair<std::string, unsigned char>> code_unsigned char_vec;
  LOG(INFO) << "\n";
  for (unsigned char c = 'a'; c <= 'z'; ++c) {
    size_t i = IndexOfLetter(c);
    LOG(INFO) << "\t" << c << ": " << distr[i] << "\t " << codes[i];
    code_unsigned char_vec.emplace_back(codes[i], c);
  }
  std::sort(code_unsigned char_vec.begin(), code_unsigned char_vec.end(), [](const std::pair<std::string, unsigned char>& a,
                                                           const std::pair<std::string, unsigned char>& b) {
    if (a.first.length() != b.first.length()) return a.first.length() < b.first.length();
    if (a.first != b.first) return a.first < b.first;
    return a.second < b.second;
  });
  LOG(INFO) << "\n";
  for (const auto& p : code_unsigned char_vec) {
    size_t i = IndexOfLetter(p.second);
    LOG(INFO) << "\t" << p.second << ": " << distr[i] << "\t " << p.first;
  }
  */
}

struct Triple {
  std::string token = "";
  float weight = 0.0f;
  std::string code = "";
};

void PrintTriple(const Triple& t) {
  LOG(INFO) << t.token << ": \t" << t.weight << "\t " << t.code;;
}

void PrintCodes(const Huffman& huff, const std::string& tokens, const std::vector<float>& distribution) {
  if (tokens.size() != distribution.size() || tokens.size() != huff.VocabularySize()) {
    return;
  }
  std::vector<Triple> triples;
  triples.reserve(tokens.size());
  for (size_t i = 0; i < tokens.size(); ++i) {
    Triple t;
    t.token.push_back(tokens[i]);
    t.weight = distribution[i];
    t.code = huff.Code(tokens[i]);
    triples.push_back(std::move(t));
  }
  Triple escape;
  escape.token = "ESC";
  escape.code = huff.EscapeCode();
  triples.push_back(std::move(escape));

  LOG(INFO) << "\n";
  for (const Triple& t : triples) {
    PrintTriple(t);
  }
  
  std::sort(triples.begin(), triples.end(), [](const Triple& a, const Triple& b){
    if (a.code.size() != b.code.size()) return a.code.size() < b.code.size();
    return a.code < b.code;
  });

  LOG(INFO) << "\n";
  for (const Triple& t : triples) {
    PrintTriple(t);
  }
}

TEST(huffman_print_english) {
  auto distr = EnglishLetterDistribution();
  const std::string tokens = "abcdefghijklmnopqrstuvwxyz";
  Huffman huff(distr, tokens);
  PrintCodes(huff, tokens, distr);
}

}  // namespace huffman
