#pragma once

#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "Dictionary.hpp"
#include "Errors.hpp"
#include "MyTypes.hpp"

template <typename T>
auto extractFields(const T& value) {
    return value;
}

template <>
inline auto extractFields<User>(const User& user) {
    return std::make_tuple(user.name, user.age, user.email, user.id);
}

template <class T>
class Set {
public:
    using value_type = T;
    using metadata_type = decltype(extractFields(std::declval<T>()));

    Set();
    explicit Set(std::unique_ptr<IDictionary<T, metadata_type>> dictionary);
    explicit Set(const std::vector<T>& values);
    Set(const Set& other);
    Set(Set&& other) noexcept;
    Set& operator=(const Set& other);
    Set& operator=(Set&& other) noexcept;
    ~Set();

    void insert(const T& value);
    bool erase(const T& value);
    bool contains(const T& value) const;
    std::optional<metadata_type> try_get(const T& value) const;
    metadata_type get(const T& value) const;
    std::size_t size() const;
    std::vector<T> values() const;
    void clear();

    Set set_union(const Set& other) const;
    Set set_intersection(const Set& other) const;
    Set set_difference(const Set& other) const;
    Set set_symdiff(const Set& other) const;

    Set operator|(const Set& rhs) const;
    Set operator&(const Set& rhs) const;
    Set operator-(const Set& rhs) const;
    Set operator^(const Set& rhs) const;

    void swap(Set& other) noexcept;

private:
    std::unique_ptr<IDictionary<T, metadata_type>> dictionary_;
};

template <class T>
void swap(Set<T>& lhs, Set<T>& rhs) noexcept {
    lhs.swap(rhs);
}

template <class T>
Set<T>::Set()
    : dictionary_(std::make_unique<Dictionary<T, metadata_type>>()) {}

template <class T>
Set<T>::Set(std::unique_ptr<IDictionary<T, metadata_type>> dictionary)
    : dictionary_(std::move(dictionary)) {
    if (!dictionary_) {
        dictionary_ = std::make_unique<Dictionary<T, metadata_type>>();
    }
}

template <class T>
Set<T>::Set(const std::vector<T>& values)
    : Set() {
    for (const auto& value : values) {
        insert(value);
    }
}

template <class T>
Set<T>::Set(const Set& other)
    : Set() {
    for (const auto& key : other.values()) {
        insert(key);
    }
}

template <class T>
Set<T>::Set(Set&& other) noexcept = default;

template <class T>
Set<T>& Set<T>::operator=(const Set& other) {
    if (this == &other) {
        return *this;
    }
    Set tmp(other);
    swap(tmp);
    return *this;
}

template <class T>
Set<T>& Set<T>::operator=(Set&& other) noexcept = default;

template <class T>
Set<T>::~Set() = default;

template <class T>
void Set<T>::insert(const T& value) {
    dictionary_->insert(value, extractFields(value));
}

template <class T>
bool Set<T>::erase(const T& value) {
    return dictionary_->erase(value);
}

template <class T>
bool Set<T>::contains(const T& value) const {
    return dictionary_->contains(value);
}

template <class T>
std::optional<typename Set<T>::metadata_type> Set<T>::try_get(const T& value) const {
    return dictionary_->get(value);
}

template <class T>
typename Set<T>::metadata_type Set<T>::get(const T& value) const {
    auto opt = dictionary_->get(value);
    if (!opt.has_value()) {
        throw KeyNotFoundError("set::get");
    }
    return *opt;
}

template <class T>
std::size_t Set<T>::size() const {
    return dictionary_->size();
}

template <class T>
std::vector<T> Set<T>::values() const {
    return dictionary_->keys();
}

template <class T>
void Set<T>::clear() {
    dictionary_->clear();
}

template <class T>
Set<T> Set<T>::set_union(const Set& other) const {
    Set result;
    for (const auto& key : values()) {
        result.insert(key);
    }
    for (const auto& key : other.values()) {
        result.insert(key);
    }
    return result;
}

template <class T>
Set<T> Set<T>::set_intersection(const Set& other) const {
    Set result;
    for (const auto& key : values()) {
        if (other.contains(key)) {
            result.insert(key);
        }
    }
    return result;
}

template <class T>
Set<T> Set<T>::set_difference(const Set& other) const {
    Set result;
    for (const auto& key : values()) {
        if (!other.contains(key)) {
            result.insert(key);
        }
    }
    return result;
}

template <class T>
Set<T> Set<T>::set_symdiff(const Set& other) const {
    Set result;
    for (const auto& key : values()) {
        if (!other.contains(key)) {
            result.insert(key);
        }
    }
    for (const auto& key : other.values()) {
        if (!contains(key)) {
            result.insert(key);
        }
    }
    return result;
}

template <class T>
Set<T> Set<T>::operator|(const Set& rhs) const {
    return set_union(rhs);
}

template <class T>
Set<T> Set<T>::operator&(const Set& rhs) const {
    return set_intersection(rhs);
}

template <class T>
Set<T> Set<T>::operator-(const Set& rhs) const {
    return set_difference(rhs);
}

template <class T>
Set<T> Set<T>::operator^(const Set& rhs) const {
    return set_symdiff(rhs);
}

template <class T>
void Set<T>::swap(Set& other) noexcept {
    using std::swap;
    swap(dictionary_, other.dictionary_);
}

