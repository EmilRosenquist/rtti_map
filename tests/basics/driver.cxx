#include <stdexcept>
#include <memory>

#include <rtti_map/version.hxx>
#include <rtti_map/rtti_map.hxx>

#undef NDEBUG
#include <cassert>

int main ()
{
  {
    const auto map = std::make_unique<rtti::map>();
    assert (map != nullptr);
  }
}
