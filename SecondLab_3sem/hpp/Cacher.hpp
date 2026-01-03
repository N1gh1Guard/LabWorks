#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>

#include "Dictionary.hpp"

template <typename K, typename V>
class LRUCache final {
private:
    struct Node {
        K key;
        V value;
        Node* prev{nullptr};
        Node* next{nullptr};

        Node(const K& k, const V& v);
    };

    std::size_t capacity_{0};
    Node*       head_{nullptr};
    Node*       tail_{nullptr};
    Dictionary<K, Node*> index_;

    void attach_front(Node* node);
    void detach(Node* node);
    void move_to_front(Node* node);
    void evict_lru();
    void destroy_nodes();

public:
    explicit LRUCache(std::size_t capacity);

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    LRUCache(LRUCache&& other) noexcept;
    LRUCache& operator=(LRUCache&& other) noexcept;
    ~LRUCache();

    bool contains(const K& key) const;
    V get(const K& key);
    void put(const K& key, const V& value);
    std::size_t size() const;
    void clear();
    bool erase(const K& key);
};

template <typename K, typename V>
LRUCache<K, V>::Node::Node(const K& k, const V& v)
    : key(k), value(v), prev(nullptr), next(nullptr) {}

template <typename K, typename V>
LRUCache<K, V>::LRUCache(std::size_t capacity)
    : capacity_(capacity) {
    if (capacity_ == 0) {
        throw std::invalid_argument("LRUCache capacity must be positive");
    }
}

template <typename K, typename V>
LRUCache<K, V>::LRUCache(LRUCache&& other) noexcept
    : capacity_(other.capacity_), head_(other.head_), tail_(other.tail_), index_(std::move(other.index_)) {
    other.head_ = other.tail_ = nullptr;
    other.capacity_ = 0;
}

template <typename K, typename V>
LRUCache<K, V>& LRUCache<K, V>::operator=(LRUCache&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    destroy_nodes();
    capacity_ = other.capacity_;
    head_ = other.head_;
    tail_ = other.tail_;
    index_ = std::move(other.index_);
    other.head_ = other.tail_ = nullptr;
    other.capacity_ = 0;
    return *this;
}

template <typename K, typename V>
LRUCache<K, V>::~LRUCache() {
    destroy_nodes();
}

template <typename K, typename V>
bool LRUCache<K, V>::contains(const K& key) const {
    return index_.contains(key);
}

template <typename K, typename V>
V LRUCache<K, V>::get(const K& key) {
    if (!contains(key)) {
        throw std::out_of_range("LRUCache::get - key not found"); // Удалить
    }
    Node* node = index_.at(key);
    move_to_front(node);
    return node->value;
}

template <typename K, typename V>
void LRUCache<K, V>::put(const K& key, const V& value) {
    if (contains(key)) {
        Node* node = index_.at(key);
        node->value = value;
        move_to_front(node);
        return;
    }

    Node* node = new Node(key, value);
    attach_front(node);
    index_.insert(key, node);

    if (index_.size() > capacity_) {
        evict_lru();
    }
}

template <typename K, typename V>
std::size_t LRUCache<K, V>::size() const {
    return index_.size();
}

template <typename K, typename V>
void LRUCache<K, V>::clear() {
    destroy_nodes();
}

template <typename K, typename V>
bool LRUCache<K, V>::erase(const K& key) {
    if (!contains(key)) {
        return false;
    }
    Node* node = index_.at(key);
    detach(node);
    index_.erase(key);
    delete node;
    return true;
}

template <typename K, typename V>
void LRUCache<K, V>::attach_front(Node* node) {
    node->prev = nullptr;
    node->next = head_;
    if (head_) {
        head_->prev = node;
    }
    head_ = node;
    if (!tail_) {
        tail_ = node;
    }
}

template <typename K, typename V>
void LRUCache<K, V>::detach(Node* node) {
    if (!node) {
        return;
    }
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        head_ = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        tail_ = node->prev;
    }
    node->prev = nullptr;
    node->next = nullptr;
}

template <typename K, typename V>
void LRUCache<K, V>::move_to_front(Node* node) {
    if (!node || node == head_) {
        return;
    }
    detach(node);
    attach_front(node);
}

template <typename K, typename V>
void LRUCache<K, V>::evict_lru() {
    if (!tail_) {
        return;
    }
    Node* victim = tail_;
    detach(victim);
    index_.erase(victim->key);
    delete victim;
}

template <typename K, typename V>
void LRUCache<K, V>::destroy_nodes() {
    Node* current = head_;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head_ = tail_ = nullptr;
    index_.clear();
}

