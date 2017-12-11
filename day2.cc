#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>

#ifdef UNITTEST
#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#else
#include <fstream>
#include <iostream>
#endif


std::vector<int> GetNumbers(const std::string text)
{
  std::istringstream ss(text);
  int num;
  std::vector<int> ret;
  while (ss >> num)
  {
    ret.push_back(num);
  }
  return ret;
}


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

template<typename RowCalc>
int GetDocumentChecksum(std::vector<std::string> lines)
{
  int checksum = 0;
  for (const std::string& line : lines)
  {
    const auto numbers = GetNumbers(line);
    const int value = RowCalc::CalculateValue(numbers);
    checksum += value;
  }
  return checksum;
}

int CalculateRowDifference(const std::vector<int>& numbers)
{
  const auto a = std::max_element(numbers.begin(), numbers.end());
  const auto i = std::min_element(numbers.begin(), numbers.end());
  return std::abs(*a - *i);
}

struct Part1
{
  static int CalculateValue(const std::vector<int>& numbers)
  {
    return CalculateRowDifference(numbers);
  }
};

int CalculateEvenlyDivided(const std::vector<int>& numbers)
{
  std::vector<int> result;
  
  const int s = numbers.size();
  for (int i = 0; i < s; i += 1)
  {
    for (int j = 0; j < s; j += 1)
    {
      if (i != j)
      {
        const int a = numbers[i];
        const int b = numbers[j];
        if (a%b == 0)
        {
          result.push_back(a / b);
        }
      }
    }
  }
  
  assert(result.size() == 1);
  return result[0];
}

struct Part2
{
  static int CalculateValue(const std::vector<int>& numbers)
  {
    return CalculateEvenlyDivided(numbers);
  }
};

#ifdef UNITTEST
TEST_CASE("Example 1") {
  const std::string row1 = "5 1 9 5";
  SECTION("Row 1") { REQUIRE(8 == CalculateRowDifference(GetNumbers(row1))); }

  const std::string row2 = "7 5 3";
  SECTION("Row 2") { REQUIRE(4 == CalculateRowDifference(GetNumbers(row2))); }

  const std::string row3 = "2 4 6 8";
  SECTION("Row 3") { REQUIRE(6 == CalculateRowDifference(GetNumbers(row3))); }

  const std::string nl = "\n";
  std::string doc = row1 + nl + row2 + nl + row3 + nl;
  SECTION("Complete")
  {
    std::istringstream ss(doc);
    REQUIRE(18 == GetDocumentChecksum<Part1>(GetLines(&ss)));
  }
}

TEST_CASE("Example 2") {
  const std::string row1 = "5 9 2 8";
  SECTION("Row 1") { REQUIRE(4 == CalculateEvenlyDivided(GetNumbers(row1))); }

  const std::string row2 = "9 4 7 3";
  SECTION("Row 2") { REQUIRE(3 == CalculateEvenlyDivided(GetNumbers(row2))); }

  const std::string row3 = "3 8 6 5";
  SECTION("Row 3") { REQUIRE(2 == CalculateEvenlyDivided(GetNumbers(row3))); }

  const std::string nl = "\n";
  std::string doc = row1 + nl + row2 + nl + row3 + nl;
  SECTION("Complete")
  {
    std::istringstream ss(doc);
    REQUIRE(9 == GetDocumentChecksum<Part2>(GetLines(&ss)));
  }
}

#else
int main()
{
  std::ifstream input_file("day2-input.txt");
  if (!input_file)
  {
    std::cerr << "Failed to open input\n";
    return 1;
  }

  const auto lines = GetLines(&input_file);
  const int check1 = GetDocumentChecksum<Part1>(lines);
  const int check2 = GetDocumentChecksum<Part2>(lines);
  std::cout << "Document checksum (part 1) " << check1 << "\n";
  std::cout << "Document checksum (part 2) " << check2 << "\n";
  return 0;
}
#endif
