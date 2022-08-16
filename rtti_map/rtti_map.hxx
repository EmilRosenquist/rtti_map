#pragma once

#include <algorithm>
#include <any>
#include <functional>
#include <iterator>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>

namespace rtti
{
  namespace internal
  {
    template <typename std_container>
    struct container
    {
      // Capacity
      [[nodiscard]] auto empty() const noexcept { return map_.empty(); }
      [[nodiscard]] auto size() const noexcept { return map_.size(); }

      // Modifiers
      auto clear() noexcept { map_.clear(); }
      template <typename T>
      auto erase() noexcept { return map_.erase(typeid(T).hash_code()); }

      // Lookup
      template <typename T>
      [[nodiscard]] auto count() const { return map_.count(typeid(T).hash_code()); }
      template <typename T>
      [[nodiscard]] auto contains() const { return map_.contains(typeid(T).hash_code()); }
      template <typename T> requires (!std::is_reference_v<T>)
      [[nodiscard]] auto equal_range()
      {
        const auto range = map_.equal_range(typeid(T).hash_code());
        return equal_range_internal<T>(range.first, range.second);
      }
      template <typename T> requires (!std::is_reference_v<T>)
      [[nodiscard]] auto equal_range() const
      {
        const auto range = map_.equal_range(typeid(T).hash_code());
        return equal_range_internal<std::add_const_t<T>>(range.first, range.second);
      }

      // Hash policy
      auto reserve(std::size_t max) { map_.reserve(max); }

    protected:
      template <typename T>
      auto insert_internal(T&& t) { return map_.insert({ typeid(T).hash_code(), t }); }
      template <typename T, typename... Args>
      auto emplace_internal(Args&&... args)
      {
        auto t = std::any{};
        t.emplace<T>(args...);
        return map_.emplace(typeid(T).hash_code(), std::move(t));
      }
      std_container map_;

    private:
      template <typename T>
      [[nodiscard]] auto equal_range_internal(auto begin, auto end) const
      {
        if constexpr (std::is_object_v<T> || std::is_function_v<T>)
        {
          return create_range<T, std::reference_wrapper<T>>(begin, end);
        }
        else
        {
          return create_range<T>(begin, end);
        }
      }
      template <typename T, typename reference_type = T>
      [[nodiscard]] auto create_range(auto begin, auto end) const
      {
        auto referenceRange = std::vector<reference_type>{};
        referenceRange.reserve(std::distance(begin, end));
        std::transform(begin, end, std::back_inserter(referenceRange), [](auto& match)
        {
          return std::ref(std::any_cast<T&>(match.second));
        });
        return referenceRange;
      }
    };
  }

  struct map : internal::container<std::unordered_map<std::size_t, std::any>>
  {
    // Modifiers
    template <typename T>
    auto insert(T&& t) { return insert_internal(t).second; }
    template <typename T>
    auto insert_or_assign(T&& t) { return map_.insert_or_assign(typeid(T).hash_code(), t).second; }
    template <typename T, typename... Args>
    auto emplace(Args&&... args) { return emplace_internal<T>(args...).second; }

    // Lookup
    template <typename T>
    [[nodiscard]] T& at() { return std::any_cast<T&>(map_.at(typeid(T).hash_code())); }
    template <typename T>
    [[nodiscard]] const T& at() const { return std::any_cast<const T&>(map_.at(typeid(T).hash_code())); }
  };

  struct multimap : internal::container<std::unordered_multimap<std::size_t, std::any>>
  {
    // Modifiers
    template <typename T>
    auto insert(T&& t) { insert_internal(t); }
    template <typename T, typename... Args>
    auto emplace(Args&&... args) { emplace_internal<T>(args...); }
  };
}
