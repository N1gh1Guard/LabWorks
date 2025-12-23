#pragma once

#include <chrono>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "AlphabetIndex.hpp"
#include "Cacher.hpp"
#include "HeavyDB.hpp"
#include "Errors.hpp"

// Глобальные экземпляры для GUI 
template <typename K, typename V>
class DatabaseManager {
private:
    HeavyDB<K, V> db_;
    LRUCache<K, V> cache_;

public:
    DatabaseManager(std::size_t cache_capacity = 100,
                    std::chrono::milliseconds db_latency = std::chrono::milliseconds{1},
                    std::size_t cpu_iters = 2000)
        : db_(db_latency, cpu_iters), cache_(cache_capacity) {}

    // Получить значение (сначала из кэша, потом из БД)
    std::optional<V> get(const K& key) {
        if (cache_.contains(key)) {
            return cache_.get(key);
        }
        try {
            V value = db_.get(key);
            cache_.put(key, value);
            return value;
        } catch (const KeyNotFoundError&) {
            return std::nullopt;
        }
    }

    // Добавить/обновить значение
    void put(const K& key, const V& value) {
        db_.put(key, value);
        if (cache_.contains(key)) {
            cache_.put(key, value);
        }
    }

    // Удалить значение
    bool erase(const K& key) {
        bool db_result = db_.erase(key);
        if (db_result) {
            cache_.erase(key);
        }
        return db_result;
    }

    // Проверить наличие ключа
    bool contains(const K& key) {
        if (cache_.contains(key)) {
            return true;
        }
        return db_.contains(key);
    }

    // Получить размер БД
    std::size_t db_size() const {
        return db_.size();
    }

    // Получить размер кэша
    std::size_t cache_size() const {
        return cache_.size();
    }

    // Очистить БД
    void clear_db() {
        db_.clear();
        cache_.clear();
    }

    // Очистить кэш
    void clear_cache() {
        cache_.clear();
    }

    // Получить все ключи из БД
    std::vector<K> get_all_keys() const {
        return db_.keys();
    }
};

// Функции для работы с алфавитным указателем
class AlphabetIndexManager {
private:
    std::unique_ptr<AlphabetIndex<std::string>> index_;

public:
    // Создать индекс из текста
    void build_index(const std::string& text, std::size_t page_size, PageSizeType type) {
        index_ = std::make_unique<AlphabetIndex<std::string>>(text, page_size, type);
    }

    // Перестроить индекс
    void rebuild(const std::string& text) {
        if (index_) {
            index_->rebuild(text);
        }
    }

    // Получить страницы
    std::vector<Page> get_pages() const {
        if (index_) {
            return index_->pages();
        }
        return {};
    }

    // Поиск слова в указателе
    std::vector<int> lookup_word(const std::string& word) const {
        if (index_) {
            return index_->lookup(word);
        }
        return {};
    }

    // Получить указатель (слово -> страницы)
    std::string get_pointer_string() const {
        if (!index_) {
            return "";
        }
        std::ostringstream oss;
        index_->printPointer(oss);
        return oss.str();
    }

    // Получить страницы в виде строки
    std::string get_pages_string() const {
        if (!index_) {
            return "";
        }
        std::ostringstream oss;
        index_->printPages(oss);
        return oss.str();
    }

    // Проверить, создан ли индекс
    bool is_ready() const {
        return index_ != nullptr;
    }
};

