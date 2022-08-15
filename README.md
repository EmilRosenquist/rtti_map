[![Cpp Standard](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)

# rtti_map
Container for using types as key and instance as value. Uses RTTI (Run-Time Type Information) to determine hash for keys for types. 

## TL;DR:
```c++
#include <rtti_map/rtti_map.hxx>

int main() 
{
    auto map = rtti::map{};

    // insert/emplace
    map.insert<int>(1);
    map.insert_or_assign<double>(1.0);

    struct A { int b, c; };
    map.emplace<A>(1, 2);

    const auto hasInt = map.contains<int>(); // true
    auto myInt = map.at<int>(); // copy
    const auto& myDouble = map.at<double>(); // const reference
    auto& myA = map.at<A>(); // reference
}
```