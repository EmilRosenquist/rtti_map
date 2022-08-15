#pragma once

#include <any>
#include <typeinfo>
#include <unordered_map>

namespace rtti
{
  struct map
  {
    // Capacity
    [[nodiscard]] auto empty() const noexcept { return map_.empty(); }
    [[nodiscard]] auto size() const noexcept { return map_.size(); }

    // Modifiers
    auto clear() noexcept { map_.clear(); }
    template <typename T>
    auto insert(T&& t)
    {
        return map_.insert({ typeid(T).hash_code(), t }).second;
    }
    template <typename T>
    auto insert_or_assign(T&& t)
    {
        return map_.insert_or_assign(typeid(T).hash_code(), t).second;
    }
    template <typename T, typename... Args>
    auto emplace(Args&&... args)
    {
        auto t = std::any{};
        t.emplace<T>(args...);
        return map_.emplace(typeid(T).hash_code(), std::move(t)).second;
    }
    template <typename T>
    auto erase() noexcept { return map_.erase(typeid(T).hash_code()); }

    // Lookup
    template <typename T>
    [[nodiscard]] T& at() { return std::any_cast<T&>(map_.at(typeid(T).hash_code())); }
    template <typename T>
    [[nodiscard]] const T& at() const { return std::any_cast<const T&>(map_.at(typeid(T).hash_code())); }
    template <typename T>
    [[nodiscard]] auto count() const { return map_.count(typeid(T).hash_code()); }
    template <typename T>
    [[nodiscard]] auto contains() const { return map_.contains(typeid(T).hash_code()); }

    // Hash policy
    auto reserve(std::size_t max) { map_.reserve(max); }
  private:
    std::unordered_map<std::size_t, std::any> map_;
  };
}
