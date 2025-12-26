#pragma once

#include <chrono>
#include <cstdint>
#include <thread>
#include <vector>

#include "Dictionary.hpp"
#include "Errors.hpp"

template <typename K, typename V>
class HeavyDB {
private:
    Dictionary<K, V>             store_;
    std::chrono::milliseconds    latency_;
    std::size_t                  cpu_iters_;

    [[nodiscard]] static std::chrono::milliseconds default_latency();
    static void burn_cpu(std::size_t iters);
    void simulate_latency() const;

public:
    explicit HeavyDB(std::chrono::milliseconds latency = default_latency(), std::size_t cpu_iters = 2'000);

    bool contains(const K& key) const;
    void put(const K& key, const V& value);
    V get(const K& key) const;
    bool erase(const K& key);
    std::size_t size() const;
    void clear();
    std::vector<K> keys() const;
};

template <typename K, typename V>
HeavyDB<K, V>::HeavyDB(std::chrono::milliseconds latency, std::size_t cpu_iters)
    : store_(), latency_(latency), cpu_iters_(cpu_iters) {}

template <typename K, typename V>
bool HeavyDB<K, V>::contains(const K& key) const {
    simulate_latency();
    return store_.contains(key);
}

template <typename K, typename V>
void HeavyDB<K, V>::put(const K& key, const V& value) {
    simulate_latency();
    store_.insert(key, value);
}

template <typename K, typename V>
V HeavyDB<K, V>::get(const K& key) const {
    simulate_latency();
    auto value = store_.get(key);
    if (!value.has_value()) {
        throw KeyNotFoundError("HeavyDB::get");
    }
    return *value;
}

template <typename K, typename V>
bool HeavyDB<K, V>::erase(const K& key) {
    simulate_latency();
    return store_.erase(key);
}

template <typename K, typename V>
std::size_t HeavyDB<K, V>::size() const {
    simulate_latency();
    return store_.size();
}

template <typename K, typename V>
void HeavyDB<K, V>::clear() {
    simulate_latency();
    store_.clear();
}

template <typename K, typename V>
std::vector<K> HeavyDB<K, V>::keys() const {
    simulate_latency();
    return store_.keys();
}

template <typename K, typename V>
std::chrono::milliseconds HeavyDB<K, V>::default_latency() {
    return std::chrono::milliseconds{1};
}

template <typename K, typename V>
void HeavyDB<K, V>::burn_cpu(std::size_t iters) {
    volatile std::uint64_t x = 0x9e3779b97f4a7c15ULL;
    volatile std::uint64_t y = 0xbf58476d1ce4e5b9ULL;
    for (std::size_t i = 0; i < iters; ++i) {
        x ^= (x << 13);
        x ^= (x >> 7);
        x ^= (x << 17);
        y += x + 0x27d4eb2f165667c5ULL;
        x += y ^ (x >> 3);
    }
    (void)x;
    (void)y;
}

template <typename K, typename V>
void HeavyDB<K, V>::simulate_latency() const {
    if (latency_.count() > 0) {
        std::this_thread::sleep_for(latency_);
    }
    if (cpu_iters_ > 0) {
        burn_cpu(cpu_iters_);
    }
}

