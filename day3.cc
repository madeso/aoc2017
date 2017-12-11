#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <map>

#ifdef UNITTEST
#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#else
#include <fstream>
#include <iostream>
#endif

struct V
{
  int x;
  int y;
  V(int ax, int ay) : x(ax), y(ay) {}
};
bool operator==(const V& lhs, const V& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}
template<typename T>
T& operator<<(T& t, const V& v)
{
  t << "(" << v.x << ", " << v.y << ")";
  return t;
}

V operator+(const V& lhs, const V& rhs)
{
  return V{ lhs.x + rhs.x, lhs.y + rhs.y };
}

struct SpiralMemoryBase
{
  int width = 1;
  int height = 1;
  int value = 1;
  V position = V{ 0, 0 };
  int direction = 0;
};

template<typename ValueCalculator>
struct SpiralMemory : public SpiralMemoryBase
{
  ValueCalculator calculator;

  void new_value()
  {
    value = calculator.Calculate(*this);
  }

  void next()
  {
    calculator.Remember(*this);

    const std::vector<V> directions = { V(1, 0), V(0, 1), V(-1, 0), V(0, -1) };
    const auto next = position + directions[direction];
    if (next.x > width) {
    }
    else if (next.x < -width)
    {
      width += 1;
    }
    else if (next.y > height)
    {
    }
    else if (next.y < -height)
    {
      height += 1;
    }
    else
    {
      position = next;
      new_value();
      return;
    }
    // switch direction and continue
    direction = (direction + 1) % 4;
    position = position + directions[direction];
    new_value();
  }
};

struct IncrementalMemory
{
  void Remember(const SpiralMemoryBase& mem){}
  int Calculate(const SpiralMemoryBase& mem)
  {
    return mem.value + 1;
  }
};

template<typename Mem>
SpiralMemory<Mem> MemoryAt(int target)
{
  SpiralMemory<Mem> mem;

  while (target != mem.value)
  {
    mem.next();
  }

  return mem;
}

template<typename Mem>
SpiralMemory<Mem> MemoryAt(const V& target)
{
  SpiralMemory<Mem> mem;

  while (!(target == mem.position))
  {
    mem.next();
  }

  return mem;
}

int Closer(int a)
{
  if (a > 0) return a - 1;
  else return a + 1;
}

std::string to_string(const V& v)
{
  std::ostringstream ss;
  ss << v;
  return ss.str();
}

struct RelativeMemory
{
  std::map<std::string, int> memory;
  void Remember(const SpiralMemoryBase& mem)
  {
    memory.insert(std::make_pair(to_string(mem.position), mem.value));
  }

  int ValueOf(const V& v) const
  {
    if (v == V{ 0, 0 }) return 1;
    const auto k = to_string(v);
    const auto r = memory.find(k);
    if (r == memory.end()) return 0;
    else return r->second;
  }

  int Calculate(const SpiralMemoryBase& mem)
  {
    if (mem.position == V{ 0, 0 }) return 1;

    int v = 0;
    for (int dx = -1; dx <= 1; dx += 1)
    {
      for (int dy = -1; dy <= 1; dy += 1)
      {
        if (dx != 0 || dy != 0)
        {
          v += ValueOf(mem.position + V{ dx, dy });
        }
      }
    }
    return v;
  }
};

int CalculateDistance(int target)
{
  const auto mem = MemoryAt<IncrementalMemory>(target);
  return std::abs(mem.position.x) + std::abs(mem.position.y);
}

int FirstValueLargerThan(int target)
{
  SpiralMemory<RelativeMemory> mem;

  while (mem.value <= target)
  {
    mem.next();
  }

  return mem.value;
}

#ifdef UNITTEST
TEST_CASE("Step 1.1") {REQUIRE(V(2, 0) == MemoryAt<IncrementalMemory>(11).position); }
TEST_CASE("Step 1.2") {REQUIRE(V(2, 2) == MemoryAt<IncrementalMemory>(13).position); }
TEST_CASE("Step 1.3") {REQUIRE(V(-2, -2) == MemoryAt<IncrementalMemory>(21).position); }
TEST_CASE("Step 1.4") {REQUIRE(V(-2, 2) == MemoryAt<IncrementalMemory>(17).position); }

TEST_CASE("Example 1.1") {REQUIRE(0 == CalculateDistance(1)); }
TEST_CASE("Example 1.2") {REQUIRE(3 == CalculateDistance(12)); }
TEST_CASE("Example 1.3") {REQUIRE(2 == CalculateDistance(23)); }
TEST_CASE("Example 1.4") {REQUIRE(31 == CalculateDistance(1024)); }

TEST_CASE("Step 2.1") { REQUIRE(1 == MemoryAt<RelativeMemory>(V(0, 0)).value); }
TEST_CASE("Step 2.2") { REQUIRE(1 == MemoryAt<RelativeMemory>(V(1, 0)).value); }
TEST_CASE("Step 2.3") { REQUIRE(2 == MemoryAt<RelativeMemory>(V(1, 1)).value); }
TEST_CASE("Step 2.4") { REQUIRE(4 == MemoryAt<RelativeMemory>(V(0, 1)).value); }

TEST_CASE("Example 2.1") { REQUIRE(54 == FirstValueLargerThan(50)); }
TEST_CASE("Example 2.2") { REQUIRE(57 == FirstValueLargerThan(55)); }
TEST_CASE("Example 2.3") { REQUIRE(133 == FirstValueLargerThan(123)); }
TEST_CASE("Example 2.4") { REQUIRE(806 == FirstValueLargerThan(748)); }
#else
int main()
{
  const int input = 347991;
  const auto dist = CalculateDistance(input);
  const auto first = FirstValueLargerThan(input);
  std::cout << "Distance (part 1) " << dist << "\n";
  std::cout << "First value larger than input (part 2) " << first << "\n";
  return 0;
}
#endif
