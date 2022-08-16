[![Cpp Standard](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)

# rtti_map
Container for using types as key and instance as value. Uses RTTI (Run-Time Type Information) to determine hash for keys for types. 

## TL;DR:
```c++
#include <rtti_map/rtti_map.hxx>

int main() 
{
    // map
    auto map = rtti::map{};

    // insert/emplace
    map.insert<int>(1);
    map.insert_or_assign<double>(1.0);
    map.emplace<float>(1.0f);

    const auto hasInt = map.contains<int>(); // true
    auto myInt = map.at<int>(); // copy
    const auto& myDouble = map.at<double>(); // const reference
    auto& myFloat = map.at<float>(); // reference

    // multimap
    auto multimap = rtti::multimap{};
    multimap.insert<double>(1.0);
    multimap.emplace<double>(2.0);

    const auto hasDouble = multimap.contains<double>(); // true
    auto range = multimap.equal_range<double>(); // reference range
    auto& myDouble = range[0].get(); // reference
}
```
See [tests](tests/basics/driver.cxx) for more details.