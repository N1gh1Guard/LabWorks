#pragma once

#include <algorithm>
#include <list>
#include <optional>
#include <vector>

#include "Errors.hpp"
#include "HashFunc.hpp"

template <class K, class V>
class HashTable final {
private:
    struct BucketEntry {
        K key;
        V value;
    };

    std::vector<std::list<BucketEntry>> buckets_;
    std::size_t                         size_{0};

    static constexpr double kMaxLoadFactor = 0.75;
    static constexpr std::size_t kInitialCapacity = 16;

    std::size_t bucket_index(const K& key, std::size_t capacity) const;
    std::size_t bucket_index(const K& key) const;
    void rehash(std::size_t new_capacity);
    void ensure_capacity();
    typename std::list<BucketEntry>::iterator find_in_bucket(std::size_t idx, const K& key);
    typename std::list<BucketEntry>::const_iterator find_in_bucket(std::size_t idx, const K& key) const;

public:
    HashTable();
    explicit HashTable(std::size_t capacity);
    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    HashTable& operator=(HashTable&& other) noexcept;
    ~HashTable();

    void insert(const K& key, const V& value);
    bool erase(const K& key);
    bool contains(const K& key) const;
    V& at(const K& key);
    const V& at(const K& key) const;
    std::optional<V> get(const K& key) const;
    std::vector<K> keys() const;
    void clear();
    std::size_t size() const;
    std::size_t capacity() const;
};

template <class K, class V>
HashTable<K, V>::HashTable()
    : buckets_(kInitialCapacity) {}

template <class K, class V>
HashTable<K, V>::HashTable(std::size_t capacity)
    : buckets_(std::max<std::size_t>(kInitialCapacity, capacity)) {}

template <class K, class V>
HashTable<K, V>::HashTable(const HashTable& other) = default;

template <class K, class V>
HashTable<K, V>& HashTable<K, V>::operator=(const HashTable& other) = default;

template <class K, class V>
HashTable<K, V>::HashTable(HashTable&& other) noexcept = default;

template <class K, class V>
HashTable<K, V>& HashTable<K, V>::operator=(HashTable&& other) noexcept = default;

template <class K, class V>
HashTable<K, V>::~HashTable() = default;

template <class K, class V>
void HashTable<K, V>::insert(const K& key, const V& value) {
    ensure_capacity();
    std::size_t idx = bucket_index(key);
    auto it = find_in_bucket(idx, key);
    if (it != buckets_[idx].end()) {
        it->value = value;
        return;
    }
    buckets_[idx].push_back(BucketEntry{key, value});
    ++size_;
}

template <class K, class V>
bool HashTable<K, V>::erase(const K& key) {
    if (buckets_.empty()) {
        return false;
    }
    std::size_t idx = bucket_index(key);
    auto it = find_in_bucket(idx, key);
    if (it == buckets_[idx].end()) {
        return false;
    }
    buckets_[idx].erase(it);
    --size_;
    return true;
}

template <class K, class V>
bool HashTable<K, V>::contains(const K& key) const {
    if (buckets_.empty()) {
        return false;
    }
    std::size_t idx = bucket_index(key);
    return find_in_bucket(idx, key) != buckets_[idx].end();
}

template <class K, class V>
V& HashTable<K, V>::at(const K& key) {
    if (buckets_.empty()) {
        throw KeyNotFoundError("<empty table>");
    }
    std::size_t idx = bucket_index(key);
    auto it = find_in_bucket(idx, key);
    if (it == buckets_[idx].end()) {
        throw KeyNotFoundError("requested key");
    }
    return it->value;
}

template <class K, class V>
const V& HashTable<K, V>::at(const K& key) const {
    if (buckets_.empty()) {
        throw KeyNotFoundError("<empty table>");
    }
    std::size_t idx = bucket_index(key);
    auto it = find_in_bucket(idx, key);
    if (it == buckets_[idx].end()) {
        throw KeyNotFoundError("requested key");
    }
    return it->value;
}

template <class K, class V>
std::optional<V> HashTable<K, V>::get(const K& key) const {
    if (buckets_.empty()) {
        return std::nullopt;
    }
    std::size_t idx = bucket_index(key);
    auto it = find_in_bucket(idx, key);
    if (it == buckets_[idx].end()) {
        return std::nullopt;
    }
    return it->value;
}

template <class K, class V>
std::vector<K> HashTable<K, V>::keys() const {
    std::vector<K> result;
    result.reserve(size_);
    for (const auto& bucket : buckets_) {
        for (const auto& entry : bucket) {
            result.push_back(entry.key);
        }
    }
    return result;
}

template <class K, class V>
void HashTable<K, V>::clear() {
    for (auto& bucket : buckets_) {
        bucket.clear();
    }
    size_ = 0;
}

template <class K, class V>
std::size_t HashTable<K, V>::size() const {
    return size_;
}

template <class K, class V>
std::size_t HashTable<K, V>::capacity() const {
    return buckets_.size();
}

template <class K, class V>
std::size_t HashTable<K, V>::bucket_index(const K& key, std::size_t capacity) const {
    return my_hash(key) % capacity;
}

template <class K, class V>
std::size_t HashTable<K, V>::bucket_index(const K& key) const {
    return bucket_index(key, buckets_.size());
}

template <class K, class V>
void HashTable<K, V>::rehash(std::size_t new_capacity) {
    std::vector<std::list<BucketEntry>> newBuckets(new_capacity);
    for (const auto& bucket : buckets_) {
        for (const auto& entry : bucket) {
            std::size_t idx = bucket_index(entry.key, new_capacity);
            newBuckets[idx].push_back(entry);
        }
    }
    buckets_.swap(newBuckets);
}

template <class K, class V>
void HashTable<K, V>::ensure_capacity() {
    if (buckets_.empty()) {
        buckets_.resize(kInitialCapacity);
        return;
    }
    const double load = static_cast<double>(size_) / static_cast<double>(buckets_.size());
    if (load > kMaxLoadFactor) {
        rehash(buckets_.size() * 2);
    }
}

template <class K, class V>
typename std::list<typename HashTable<K, V>::BucketEntry>::iterator
HashTable<K, V>::find_in_bucket(std::size_t idx, const K& key) {
    auto& bucket = buckets_[idx];
    return std::find_if(bucket.begin(), bucket.end(), [&](const BucketEntry& entry) {
        return entry.key == key;
    });
}

template <class K, class V>
typename std::list<typename HashTable<K, V>::BucketEntry>::const_iterator
HashTable<K, V>::find_in_bucket(std::size_t idx, const K& key) const {
    const auto& bucket = buckets_[idx];
    return std::find_if(bucket.begin(), bucket.end(), [&](const BucketEntry& entry) {
        return entry.key == key;
    });
}

