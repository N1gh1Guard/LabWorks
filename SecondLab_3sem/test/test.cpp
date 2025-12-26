#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <chrono>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "../hpp/AlphabetIndex.hpp"
#include "../hpp/Cacher.hpp"
#include "../hpp/Dictionary.hpp"
#include "../hpp/HashFunc.hpp"
#include "../hpp/HashTable.hpp"
#include "../hpp/HeavyDB.hpp"
#include "../hpp/MyTypes.hpp"
#include "../hpp/Set.hpp"

TEST_CASE("my_hash is deterministic and disperses adjacent integers", "[hash]") {
    constexpr int value = 42;
    REQUIRE(my_hash(value) == my_hash(value));

    std::vector<std::size_t> hits(31, 0);
    for (int i = 0; i < 10'000; ++i) {
        ++hits[my_hash(i) % hits.size()];
    }
    std::size_t non_zero = 0;
    for (auto count : hits) {
        if (count > 0) {
            ++non_zero;
        }
    }
    REQUIRE(non_zero > hits.size() * 0.9);
}

TEST_CASE("HashTable provides insert, update, lookup and erase", "[hash-table]") {
    HashTable<int, std::string> table(8);
    table.insert(1, "one");
    table.insert(2, "two");
    table.insert(33, "thirty three");

    REQUIRE(table.size() == 3);
    REQUIRE(table.contains(1));
    REQUIRE(table.contains(2));
    REQUIRE(table.contains(33));

    SECTION("get returns optional value") {
        auto v = table.get(1);
        REQUIRE(v.has_value());
        REQUIRE(*v == "one");
        REQUIRE_FALSE(table.get(999));
    }

    SECTION("update overwrites existing key") {
        table.insert(1, "uno");
        REQUIRE(table.get(1).value() == "uno");
    }

    SECTION("erase removes keys") {
        REQUIRE(table.erase(2));
        REQUIRE_FALSE(table.contains(2));
        REQUIRE(table.size() == 2);
        REQUIRE_FALSE(table.erase(999));
    }
}

TEST_CASE("Dictionary forwards to underlying HashTable", "[dictionary]") {
    Dictionary<std::string, int> dict;
    dict.insert("alpha", 1);
    dict.insert("beta", 2);
    REQUIRE(dict.size() == 2);
    REQUIRE(dict.contains("alpha"));
    REQUIRE(dict.at("alpha") == 1);
    REQUIRE(dict.erase("alpha"));
    REQUIRE_FALSE(dict.contains("alpha"));
    REQUIRE(dict.size() == 1);
}

TEST_CASE("Set<int> supports algebraic operations", "[set]") {
    Set<int> a;
    a.insert(1);
    a.insert(2);
    a.insert(2);

    Set<int> b;
    b.insert(2);
    b.insert(3);

    auto u = a | b;
    auto i = a & b;
    auto d = a - b;
    auto x = a ^ b;

    REQUIRE(u.size() == 3);
    REQUIRE(i.size() == 1);
    REQUIRE(d.size() == 1);
    REQUIRE(x.size() == 2);

    REQUIRE(u.contains(1));
    REQUIRE(u.contains(3));
    REQUIRE(i.contains(2));
    REQUIRE(d.contains(1));
    REQUIRE_FALSE(d.contains(2));
    REQUIRE(x.contains(1));
    REQUIRE(x.contains(3));
}

TEST_CASE("Set<User> stores metadata extracted from fields", "[set][user]") {
    User maria{"Maria", 28, "maria@example.com", 7};
    Set<User> users;
    users.insert(maria);

    auto info = users.try_get(maria);
    REQUIRE(info.has_value());
    REQUIRE(std::get<0>(*info) == "Maria");
    REQUIRE(std::get<1>(*info) == 28);
}

TEST_CASE("LRUCache evicts least recently used items", "[cache][lru]") {
    LRUCache<int, std::string> cache(2);
    cache.put(1, "one");
    cache.put(2, "two");
    REQUIRE(cache.size() == 2);
    REQUIRE(cache.contains(1));
    REQUIRE(cache.contains(2));

    REQUIRE(cache.get(1) == "one");
    cache.put(3, "three");
    REQUIRE_FALSE(cache.contains(2));
    REQUIRE(cache.contains(1));
    REQUIRE(cache.contains(3));

    cache.put(1, "uno");
    REQUIRE(cache.get(1) == "uno");
    cache.put(4, "four");
    REQUIRE_FALSE(cache.contains(3));
}

TEST_CASE("LRUCache get on missing key throws", "[cache]") {
    LRUCache<int, int> cache(1);
    cache.put(42, 9000);
    REQUIRE_THROWS_AS(cache.get(100), std::out_of_range);
}

TEST_CASE("HeavyDB honours latency configuration and integrates with cache", "[heavydb][cache]") {
    HeavyDB<int, std::string> db(std::chrono::milliseconds{0}, 0);
    LRUCache<int, std::string> cache(3);

    db.put(1, "one");
    db.put(2, "two");
    db.put(3, "three");

    for (int key : {1, 2, 3}) {
        auto value = db.get(key);
        cache.put(key, value);
    }

    REQUIRE(cache.contains(2));
    REQUIRE(cache.get(2) == "two");
    REQUIRE(db.size() == 3);

    db.erase(2);
    REQUIRE_FALSE(db.contains(2));
}

struct PerformanceResult {
    double avg_put_db{};
    double avg_put_cache{};
    double avg_get_db{};
    double avg_get_cache{};
    double avg_erase_db{};
    double avg_erase_cache{};
};

template <typename K, typename V>
PerformanceResult benchmark_db_vs_cache(std::size_t n_insert, std::size_t n_queries, std::size_t cache_capacity) {
    HeavyDB<K, V> db(std::chrono::milliseconds{1}, 50'000);
    LRUCache<K, V> cache(cache_capacity);

    std::vector<K> keys;
    keys.reserve(n_insert);
    for (std::size_t i = 0; i < n_insert; ++i) {
        keys.push_back(static_cast<K>(i));
    }

    std::vector<V> values;
    values.reserve(n_insert);
    for (std::size_t i = 0; i < n_insert; ++i) {
        values.emplace_back("value_" + std::to_string(i));
    }

    auto shuffled_keys = keys;
    std::shuffle(shuffled_keys.begin(), shuffled_keys.end(), std::mt19937{42});
    shuffled_keys.resize(n_queries);

    using clock = std::chrono::steady_clock;
    PerformanceResult res{};

    auto t0 = clock::now();
    for (std::size_t i = 0; i < n_insert; ++i) db.put(keys[i], values[i]);
    auto t1 = clock::now();

    auto t2 = clock::now();
    for (std::size_t i = 0; i < n_insert; ++i) cache.put(keys[i], values[i]);
    auto t3 = clock::now();

    auto t4 = clock::now();
    for (std::size_t i = 0; i < shuffled_keys.size(); ++i) {
        (void)db.get(shuffled_keys[i]);
    }
    auto t5 = clock::now();

    auto t6 = clock::now();
    for (std::size_t i = 0; i < shuffled_keys.size(); ++i) {
        (void)cache.get(shuffled_keys[i]);
    }
    auto t7 = clock::now();

    auto t8 = clock::now();
    for (std::size_t i = 0; i < shuffled_keys.size(); ++i) {
        db.erase(shuffled_keys[i]);
    }
    auto t9 = clock::now();

    auto t10 = clock::now();
    for (std::size_t i = 0; i < std::min(cache_capacity, shuffled_keys.size()); ++i) {
        cache.erase(shuffled_keys[i]);
    }
    auto t11 = clock::now();

    auto ns = [](auto duration) {
        return static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
    };

    res.avg_put_db = ns(t1 - t0) / static_cast<double>(n_insert);
    res.avg_put_cache = ns(t3 - t2) / static_cast<double>(n_insert);
    res.avg_get_db = ns(t5 - t4) / static_cast<double>(shuffled_keys.size());
    res.avg_get_cache = ns(t7 - t6) / static_cast<double>(shuffled_keys.size());
    res.avg_erase_db = ns(t9 - t8) / static_cast<double>(shuffled_keys.size());
    res.avg_erase_cache = ns(t11 - t10) / static_cast<double>(std::min(cache_capacity, shuffled_keys.size()));

    return res;
}

TEST_CASE("HeavyDB vs LRUCache performance comparison", "[perf][heavydb][cache]") {
    constexpr std::size_t inserts = 500;
    constexpr std::size_t queries = 1000;
    constexpr std::size_t cache_capacity = inserts;

    auto result = benchmark_db_vs_cache<int, std::string>(inserts, queries, cache_capacity);

    INFO("Average PUT time (ns)   - HeavyDB: " << result.avg_put_db
         << " | Cache: " << result.avg_put_cache);
    INFO("Average GET time (ns)   - HeavyDB: " << result.avg_get_db
         << " | Cache: " << result.avg_get_cache);
    INFO("Average ERASE time (ns) - HeavyDB: " << result.avg_erase_db
         << " | Cache: " << result.avg_erase_cache);

    REQUIRE(result.avg_get_cache <= result.avg_get_db * 1.2);
    REQUIRE(result.avg_put_cache <= result.avg_put_db * 1.5);
}

template <typename K, typename V>
std::optional<V> get_with_cache(LRUCache<K, V>& cache, HeavyDB<K, V>& db, const K& key) {
    if (cache.contains(key)) {
        return cache.get(key);
    }

    auto dbValue = db.get(key);
    cache.put(key, dbValue);
    return dbValue;
}

TEST_CASE("AlphabetIndex splits text, paginates and builds pointer", "[alphabet]") {
    const std::string text = "Apple banana apple carrot. Banana apple!";
    AlphabetIndex<std::string> index(text, 2, PageSizeType::BY_WORDS);

    const auto& pages = index.pages();
    REQUIRE(pages.size() >= 2);
    REQUIRE(pages.front().words.size() <= pages.front().word_limit);

    auto apple_pages = index.lookup("apple");
    REQUIRE_FALSE(apple_pages.empty());

    std::ostringstream oss;
    oss << index;
    auto printed = oss.str();
    REQUIRE(printed.find("apple") != std::string::npos);
}

