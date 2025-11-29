#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace kanji {
struct KanjiWord {
  std::string word;
  std::string reading;
};

struct KanjiData {
  std::uint32_t id;
  std::string kanji;
  std::string meaning;
  std::vector<KanjiWord> examples;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(KanjiWord, word, reading)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(KanjiData, id, kanji, examples, meaning)

};
