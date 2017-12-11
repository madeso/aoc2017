#include <string>

#ifdef UNITTEST
#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#else
#include <fstream>
#include <iostream>
#endif

template<typename Provider>
int Captcha(const std::string& input)
{
  const int size = input.size();

  int value = 0;

  for (int i = 0; i < size; i += 1)
  {
    const char current = input[i];
    const char next = input[Provider::NextIndex(i, size)];
    if (current == next)
    {
      value += current - '0';
    }
  }

  return value;
}

struct Part1
{
  static int NextIndex(int i, int size)
  {
    return (i + 1) % size;
  }
};

struct Part2
{
  static int NextIndex(int i, int size)
  {
    return (i + size / 2) % size;
  }
};

#ifdef UNITTEST
TEST_CASE("Example 1.1") { REQUIRE(3  == Captcha<Part1>("1122")); }
TEST_CASE("Example 1.2") { REQUIRE(4  == Captcha<Part1>("1111")); }
TEST_CASE("Example 1.3") { REQUIRE(0  == Captcha<Part1>("1234")); }
TEST_CASE("Example 1.4") { REQUIRE(9  == Captcha<Part1>("91212129")); }

TEST_CASE("Example 2.1") { REQUIRE(6  == Captcha<Part2>("1212")); }
TEST_CASE("Example 2.2") { REQUIRE(0  == Captcha<Part2>("1221")); }
TEST_CASE("Example 2.3") { REQUIRE(4  == Captcha<Part2>("123425")); }
TEST_CASE("Example 2.4") { REQUIRE(12 == Captcha<Part2>("123123")); }
TEST_CASE("Example 2.5") { REQUIRE(4  == Captcha<Part2>("12131415")); }

#else
int main()
{
  std::ifstream input_file("day1-input.txt");
  if (!input_file)
  {
    std::cerr << "Failed to open input\n";
    return 1;
  }
  std::string input;
  input_file >> input;
  const int c1 = Captcha<Part1>(input);
  const int c2 = Captcha<Part2>(input);
  std::cout << "Captcha 1 is " << c1 << "\n";
  std::cout << "Captcha 2 is " << c2 << "\n";
  return 0;
}
#endif
