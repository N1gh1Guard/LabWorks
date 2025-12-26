#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <type_traits>

#include "MyTypes.hpp"

inline std::uint64_t splitmix64(std::uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);
    return x;
}

template <typename T>
size_t my_hash(const T& key) {
    if constexpr (std::is_integral_v<T>) {
        return static_cast<size_t>(splitmix64(static_cast<std::uint64_t>(key)));
    } else if constexpr (std::is_pointer_v<T>) {
        return static_cast<size_t>(splitmix64(reinterpret_cast<std::uintptr_t>(key)));
    } else {
        return std::hash<T>{}(key);
    }
}

template <>
inline size_t my_hash<std::string>(const std::string& value) {
    constexpr std::uint64_t fnv_offset_basis = 14695981039346656037ULL;
    constexpr std::uint64_t fnv_prime = 1099511628211ULL;

    std::uint64_t hash = fnv_offset_basis;
    for (unsigned char c : value) {
        hash ^= c;
        hash *= fnv_prime;
    }
    return static_cast<size_t>(hash);
}

template <>
inline size_t my_hash<User>(const User& user) {
    std::size_t h = splitmix64(static_cast<std::uint64_t>(my_hash(user.name)));
    h ^= splitmix64(static_cast<std::uint64_t>(user.age) + 0x9e3779b97f4a7c15ULL);
    h ^= splitmix64(static_cast<std::uint64_t>(my_hash(user.email)) + 0x9e3779b97f4a7c15ULL);
    h ^= splitmix64(static_cast<std::uint64_t>(user.id) + 0x9e3779b97f4a7c15ULL);
    return h;
}

template <typename T1, typename T2>
size_t my_hash(const std::pair<T1, T2>& value) {
    std::size_t h1 = my_hash(value.first);
    std::size_t h2 = my_hash(value.second);
    return splitmix64(static_cast<std::uint64_t>(h1) ^ (static_cast<std::uint64_t>(h2) << 1));
}
