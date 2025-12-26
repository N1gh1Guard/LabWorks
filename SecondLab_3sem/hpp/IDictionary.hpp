#pragma once

#include <optional>
#include <vector>

template <class K, class V>
class IDictionary {
public:
    virtual ~IDictionary() = default;

    virtual void insert(const K& key, const V& value) = 0;
    virtual bool erase(const K& key) = 0;
    virtual bool contains(const K& key) const = 0;

    virtual V& at(const K& key) = 0;
    virtual const V& at(const K& key) const = 0;
    virtual std::optional<V> get(const K& key) const = 0;

    virtual std::vector<K> keys() const = 0;
    virtual void clear() = 0;

    virtual std::size_t size() const = 0;
    virtual std::size_t capacity() const = 0;
};

