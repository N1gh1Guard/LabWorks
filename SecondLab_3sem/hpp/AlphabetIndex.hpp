#pragma once

#include <algorithm>
#include <cctype>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "HashTable.hpp"
#include "MyTypes.hpp"

enum class PageSizeType {
    BY_WORDS,
    BY_CHARS
};

template <typename Key = std::string>
class AlphabetIndex {
public:
    AlphabetIndex(const std::string& text, std::size_t page_size, PageSizeType type);

    void rebuild(const std::string& text);
    const std::vector<Page>& pages() const;
    const HashTable<Key, std::vector<int>>& pointer() const;
    std::vector<int> lookup(const Key& word) const;
    void printPages(std::ostream& os) const;
    void printPointer(std::ostream& os) const;

    template <typename K>
    friend std::ostream& operator<<(std::ostream& os, const AlphabetIndex<K>& index);

private:
    PageSizeType                     type_;
    std::size_t                      page_size_;
    std::vector<std::string>         words_;
    std::vector<Page>                pages_;
    HashTable<Key, std::vector<int>> pointer_;

    static std::string normalize_word(std::string word);
    static std::vector<std::string> tokenize(const std::string& text);
    std::size_t effective_limit(std::size_t page_index) const;
    static std::size_t chars_with_spaces(const Page& page, const std::string& word);
    void paginate();
    void build_pointer();
};

template <typename Key>
AlphabetIndex<Key>::AlphabetIndex(const std::string& text, std::size_t page_size, PageSizeType type)
    : type_(type), page_size_(page_size) {
    rebuild(text);
}

template <typename Key>
void AlphabetIndex<Key>::rebuild(const std::string& text) {
    words_ = tokenize(text);
    paginate();
    build_pointer();
}

template <typename Key>
const std::vector<Page>& AlphabetIndex<Key>::pages() const {
    return pages_;
}

template <typename Key>
const HashTable<Key, std::vector<int>>& AlphabetIndex<Key>::pointer() const {
    return pointer_;
}

template <typename Key>
std::vector<int> AlphabetIndex<Key>::lookup(const Key& word) const {
    auto opt = pointer_.get(word);
    if (opt.has_value()) {
        return *opt;
    }
    return {};
}

template <typename Key>
void AlphabetIndex<Key>::printPages(std::ostream& os) const {
    for (std::size_t i = 0; i < pages_.size(); ++i) {
        const auto& page = pages_[i];
        os << "Page " << (i + 1) << " [limit=" << page.word_limit << ", used=" << page.used << "]: ";
        for (std::size_t j = 0; j < page.words.size(); ++j) {
            os << page.words[j];
            if (j + 1 < page.words.size()) {
                os << ' ';
            }
        }
        os << '\n';
    }
}

template <typename Key>
void AlphabetIndex<Key>::printPointer(std::ostream& os) const {
    auto keys = pointer_.keys();
    std::sort(keys.begin(), keys.end());
    for (const auto& key : keys) {
        os << key << " -> ";
        auto opt = pointer_.get(key);
        if (opt.has_value()) {
            const auto& pages = *opt;
            for (std::size_t i = 0; i < pages.size(); ++i) {
                os << pages[i];
                if (i + 1 < pages.size()) {
                    os << ", ";
                }
            }
        }
        os << '\n';
    }
}

template <typename Key>
std::string AlphabetIndex<Key>::normalize_word(std::string word) {
    auto left = std::find_if_not(word.begin(), word.end(), [](unsigned char c) { return std::ispunct(c); });
    auto right = std::find_if_not(word.rbegin(), word.rend(), [](unsigned char c) { return std::ispunct(c); }).base();
    if (left >= right) {
        return {};
    }
    std::string trimmed(left, right);
    std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return trimmed;
}

template <typename Key>
std::vector<std::string> AlphabetIndex<Key>::tokenize(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        auto normalized = normalize_word(token);
        if (!normalized.empty()) {
            tokens.push_back(normalized);
        }
    }
    return tokens;
}

template <typename Key>
std::size_t AlphabetIndex<Key>::effective_limit(std::size_t page_index) const {
    if (type_ == PageSizeType::BY_WORDS) {
        if (page_index == 0) {
            return std::max<std::size_t>(1, page_size_ / 2);
        }
        if ((page_index + 1) % 10 == 0) {
            return std::max<std::size_t>(1, (page_size_ * 3) / 4);
        }
        return page_size_;
    }

    if (page_index == 0) {
        return std::max<std::size_t>(1, page_size_);
    }
    if ((page_index + 1) % 10 == 0) {
        return std::max<std::size_t>(1, (page_size_ * 3) / 2);
    }
    return page_size_;
}

template <typename Key>
std::size_t AlphabetIndex<Key>::chars_with_spaces(const Page& page, const std::string& word) {
    std::size_t chars = 0;
    for (const auto& w : page.words) {
        chars += w.size();
    }
    if (!page.words.empty()) {
        chars += page.words.size() - 1;
    }
    chars += word.size();
    if (!page.words.empty()) {
        chars += 1;
    }
    return chars;
}

template <typename Key>
void AlphabetIndex<Key>::paginate() {
    pages_.clear();
    if (words_.empty()) {
        return;
    }

    Page current;
    std::size_t page_index = 0;
    current.word_limit = effective_limit(page_index);

    auto start_new_page = [&]() {
        if (!current.words.empty()) {
            pages_.push_back(current);
        }
        ++page_index;
        current = Page{};
        current.word_limit = effective_limit(page_index);
    };

    for (const auto& word : words_) {
        if (type_ == PageSizeType::BY_WORDS) {
            if (current.used >= current.word_limit && !current.words.empty()) {
                start_new_page();
            }
            if (current.words.empty()) {
                current.used = 0;
            }
            current.words.push_back(word);
            current.used += 1;
        } else {
            std::size_t required = chars_with_spaces(current, word);
            if (required > current.word_limit && !current.words.empty()) {
                start_new_page();
                required = chars_with_spaces(current, word);
            }
            if (current.words.empty()) {
                current.used = 0;
            }
            current.words.push_back(word);
            current.used = required;
        }
    }

    if (!current.words.empty()) {
        pages_.push_back(current);
    }
}

template <typename Key>
void AlphabetIndex<Key>::build_pointer() {
    pointer_.clear();
    for (std::size_t i = 0; i < pages_.size(); ++i) {
        int page_number = static_cast<int>(i + 1);
        for (const auto& word : pages_[i].words) {
            if (!pointer_.contains(word)) {
                pointer_.insert(word, std::vector<int>{page_number});
            } else {
                auto& vec = pointer_.at(word);
                if (vec.empty() || vec.back() != page_number) {
                    vec.push_back(page_number);
                }
            }
        }
    }

    auto keys = pointer_.keys();
    for (const auto& key : keys) {
        auto& vec = pointer_.at(key);
        std::sort(vec.begin(), vec.end());
        vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
    }
}

template <typename Key>
std::ostream& operator<<(std::ostream& os, const AlphabetIndex<Key>& index) {
    index.printPointer(os);
    return os;
}
