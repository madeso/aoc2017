#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>

#ifdef UNITTEST
#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#else
#include <fstream>
#include <iostream>
#endif

template<typename CharT, typename Traits>
std::vector<std::string> GetLines(std::basic_istream<CharT, Traits>* document)
{
  std::vector<std::string> lines;
  for (std::string line; std::getline(*document, line);)
  {
    lines.push_back(line);
  }
  return lines;
}

template<typename Inserter>
bool IsValidPassphrase(const std::string& pass)
{
  std::istringstream ss(pass);

  std::set<std::string> old_words;

  std::string word;
  while (ss >> word)
  {
    if (old_words.find(Inserter::Transform(word)) != old_words.end())
    {
      return false;
    }

    old_words.insert(Inserter::Transform(word));
  }

  return true;
}

struct SingleInserter
{
  static std::string Transform(const std::string& word)
  {
    return word;
  }
};

template<typename Inserter>
int GetNumberOfValidPassphrases(const std::vector<std::string>& passphrases)
{
  int valid = 0;
  for (const auto& pass : passphrases)
  {
    if (IsValidPassphrase<Inserter>(pass))
    {
      valid += 1;
    }
  }

  return valid;
}

struct AnagramInserter
{
  static std::string Transform(const std::string& word)
  {
    std::vector<char> str(word.begin(), word.end());
    std::sort(str.begin(), str.end());
    return std::string(str.begin(), str.end());
  }
};

#ifdef UNITTEST
TEST_CASE("Example 1.1") {REQUIRE(IsValidPassphrase<SingleInserter>("aa bb cc dd ee")); }
TEST_CASE("Example 1.2") {REQUIRE_FALSE(IsValidPassphrase<SingleInserter>("aa bb cc dd aa")); }
TEST_CASE("Example 1.3") {REQUIRE(IsValidPassphrase<SingleInserter>("aa bb cc dd aaa")); }

TEST_CASE("Anagram transform 1") { REQUIRE("abc" == AnagramInserter::Transform("abc")); }
TEST_CASE("Anagram transform 2") { REQUIRE("abc" == AnagramInserter::Transform("bac")); }

TEST_CASE("Example 2.1") { REQUIRE(IsValidPassphrase<AnagramInserter>("abcde fghij")); }
TEST_CASE("Example 2.2") { REQUIRE_FALSE(IsValidPassphrase<AnagramInserter>("abcde xyz ecdab")); }
TEST_CASE("Example 2.3") { REQUIRE(IsValidPassphrase<AnagramInserter>("a ab abc abd abf abj")); }
TEST_CASE("Example 2.4") { REQUIRE(IsValidPassphrase<AnagramInserter>("iiii oiii ooii oooi oooo")); }
TEST_CASE("Example 2.5") { REQUIRE_FALSE(IsValidPassphrase<AnagramInserter>("oiii ioii iioi iiio")); }
#else
int main()
{
  std::ifstream input_file("day4-input.txt");
  if (!input_file)
  {
    std::cerr << "Failed to open input\n";
    return 1;
  }

  const auto lines = GetLines(&input_file);
  const int check1 = GetNumberOfValidPassphrases<SingleInserter>(lines);
  const int check2 = GetNumberOfValidPassphrases<AnagramInserter>(lines);
  std::cout << "Number of valid passphrases (Part 1) " << check1 << "\n";
  std::cout << "Number of valid anagram passphrases (Part 2) " << check2 << "\n";
  return 0;
}
#endif
