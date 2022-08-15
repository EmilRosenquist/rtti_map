#include <stdexcept>
#include <memory>

#include <rtti_map/version.hxx>
#include <rtti_map/rtti_map.hxx>

#undef NDEBUG
#include <cassert>

int main ()
{
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

    struct A{ int b, c; };
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

    auto& elementReference = map.at<int>();
    elementReference = 2;

    assert(map.at<int>() == 2);
  }
}
