#include <stdexcept>
#include <memory>

#include <rtti_map/version.hxx>
#include <rtti_map/rtti_map.hxx>

#undef NDEBUG
#include <cassert>
#include <iostream>

int main ()
{
  // rtti::map

  // Capacity tests
  {
    auto map = rtti::map{};

    assert(map.empty());
    assert(map.size() == 0);

    map.insert<int>(1);
    assert(!map.empty());
    assert(map.size() == 1);
  }

  // Modifiers tests
  {
    auto map = rtti::map{};

    const auto insertionResult = map.insert<int>(1);
    const auto insertionResult2 = map.insert<int>(2);

    assert(map.at<int>() == 1);
    assert(insertionResult);
    assert(!insertionResult2);

    const auto insertionResult3 = map.insert_or_assign<double>(3);
    const auto insertionResult4 = map.insert_or_assign<int>(4);

    assert(map.at<double>() == 3);
    assert(map.at<int>() == 4);
    assert(insertionResult3);
    assert(!insertionResult4);

    struct A
    { 
      A(int b, int c) : b(b), c(c){}
      int b, c;
    };
    const auto emplaceResult1 = map.emplace<A>(5, 5);
    const auto emplaceResult2 = map.emplace<int>(6);

    assert(map.at<A>().b == 5 && map.at<A>().c == 5);
    assert(map.at<int>() == 4);
    assert(emplaceResult1);
    assert(!emplaceResult2);

    const auto eraseResult1 = map.erase<int>();
    const auto eraseResult2 = map.erase<float>();

    assert(eraseResult1 == 1);
    assert(eraseResult2 == 0);

    map.clear();
    assert(map.empty());
  }

  // Lookup tests
  {
    auto map = rtti::map{};

    // Empty initialized
    assert(map.count<int>() == 0);
    assert(!map.contains<int>());

    map.insert<int>(1);

    assert(map.count<int>() == 1);
    assert(map.contains<int>());
    assert(map.at<int>() == 1);

    const auto range = map.equal_range<int>();
    assert(range[0] == 1);

    auto& elementReference = map.at<int>();
    elementReference = 2;

    assert(map.at<int>() == 2);

    const auto range2 = map.equal_range<int>();
    assert(range2[0] == 2);

    auto& elementReference2 = range2[0].get();
    elementReference2 = 3;

    assert(map.at<int>() == 3);
  }

  // rtti::multimap

  // Capacity tests
  {
    auto map = rtti::multimap{};

    assert(map.empty());
    assert(map.size() == 0);

    map.insert<int>(1);
    map.insert<int>(2);
    assert(!map.empty());
    assert(map.size() == 2);
  }

  // Modifiers tests
  {
    auto map = rtti::multimap{};

    map.insert<int>(1);
    map.insert<int>(2);

    const auto range = map.equal_range<int>();
    assert(range.size() == 2);
    assert(std::count(std::cbegin(range), std::cend(range), 1) == 1);
    assert(std::count(std::cbegin(range), std::cend(range), 2) == 1);

    struct A
    {
        A(int b, int c) : b(b), c(c) {}
        int b, c;
    };
    map.emplace<A>(3, 3);
    map.emplace<int>(4);

    const auto range2 = map.equal_range<A>();
    const auto range3 = map.equal_range<int>();
    assert(range2[0].get().b == 3 && range2[0].get().c == 3);
    assert(std::count(std::cbegin(range3), std::cend(range3), 1) == 1);
    assert(std::count(std::cbegin(range3), std::cend(range3), 2) == 1);
    assert(std::count(std::cbegin(range3), std::cend(range3), 4) == 1);

    const auto eraseResult1 = map.erase<int>();
    const auto eraseResult2 = map.erase<float>();

    assert(eraseResult1 == 3);
    assert(eraseResult2 == 0);

    map.clear();
    assert(map.empty());
  }

  // Lookup tests
  {
    auto map = rtti::multimap{};

    // Empty initialized
    assert(map.count<int>() == 0);
    assert(!map.contains<int>());

    map.insert<int>(1);

    assert(map.count<int>() == 1);
    assert(map.contains<int>());

    const auto range = map.equal_range<int>();
    assert(range[0] == 1);

    auto& elementReference = range[0].get();
    elementReference = 2;

    const auto range2 = map.equal_range<int>();
    assert(range2[0] == 2);
  }
}
