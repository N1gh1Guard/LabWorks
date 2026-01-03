#pragma once

#include "HashTable.hpp"

template <typename K, typename V>
class Dictionary final {
private:
    HashTable<K, V> table_;

public:
    Dictionary();
    explicit Dictionary(std::size_t initial_capacity);

    Dictionary(const Dictionary& other);
    Dictionary& operator=(const Dictionary& other);
    Dictionary(Dictionary&& other) noexcept;
    Dictionary& operator=(Dictionary&& other) noexcept;
    ~Dictionary();

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

template <typename K, typename V>
Dictionary<K, V>::Dictionary() = default;

template <typename K, typename V>
Dictionary<K, V>::Dictionary(std::size_t initial_capacity)
    : table_(initial_capacity) {}

template <typename K, typename V>
Dictionary<K, V>::Dictionary(const Dictionary& other) = default;

template <typename K, typename V>
Dictionary<K, V>& Dictionary<K, V>::operator=(const Dictionary& other) = default;

template <typename K, typename V>
Dictionary<K, V>::Dictionary(Dictionary&& other) noexcept = default;

template <typename K, typename V>
Dictionary<K, V>& Dictionary<K, V>::operator=(Dictionary&& other) noexcept = default;

template <typename K, typename V>
Dictionary<K, V>::~Dictionary() = default;

template <typename K, typename V>
void Dictionary<K, V>::insert(const K& key, const V& value) {
    table_.insert(key, value);
}

template <typename K, typename V>
bool Dictionary<K, V>::erase(const K& key) {
    return table_.erase(key);
}

template <typename K, typename V>
bool Dictionary<K, V>::contains(const K& key) const {
    return table_.contains(key);
}

template <typename K, typename V>
V& Dictionary<K, V>::at(const K& key) {
    return table_.at(key);
}

template <typename K, typename V>
const V& Dictionary<K, V>::at(const K& key) const {
    return table_.at(key);
}

template <typename K, typename V>
std::optional<V> Dictionary<K, V>::get(const K& key) const {
    return table_.get(key);
}

template <typename K, typename V>
std::vector<K> Dictionary<K, V>::keys() const {
    return table_.keys();
}

template <typename K, typename V>
void Dictionary<K, V>::clear() {
    table_.clear();
}

template <typename K, typename V>
std::size_t Dictionary<K, V>::size() const {
    return table_.size();
}

template <typename K, typename V>
std::size_t Dictionary<K, V>::capacity() const {
    return table_.capacity();
}
