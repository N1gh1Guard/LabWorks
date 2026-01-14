#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

class BoyerMooreHorspool {
public:
    static std::unordered_map<char, int> BuildBadCharTable(const std::string& pattern) {
        std::unordered_map<char, int> table;
        int m = static_cast<int>(pattern.length());
        
        for (int i = 0; i < m - 1; ++i) {
            table[pattern[i]] = m - i - 1;
        }
        
        return table;
    }
    
    static int SearchFirst(const std::string& text, const std::string& pattern) {
        if (pattern.empty() || text.length() < pattern.length()) {
            return -1;
        }
        
        auto badCharTable = BuildBadCharTable(pattern);
        int n = static_cast<int>(text.length());
        int m = static_cast<int>(pattern.length());
        
        int i = m - 1;
        
        while (i < n) {
            int j = m - 1;
            
            // Сравниваем паттерн с текстом справа налево
            while (j >= 0 && text[i] == pattern[j]) {
                --i;
                --j;
            }
            
            // Если паттерн найден
            if (j < 0) {
                return i + 1;
            }
            
            auto it = badCharTable.find(text[i]);
            int shift = (it != badCharTable.end()) ? it->second : m;
            
            i += std::max(1, shift);
        }
        
        return -1;  // Паттерн не найден
    }
    
    // Поиск всех вхождений паттерна в текст
    static std::vector<int> SearchAll(const std::string& text, const std::string& pattern) {
        std::vector<int> results;
        
        if (pattern.empty() || text.length() < pattern.length()) {
            return results;
        }
        
        auto badCharTable = BuildBadCharTable(pattern);
        int n = static_cast<int>(text.length());
        int m = static_cast<int>(pattern.length());
        int i = m - 1;
        
        while (i < n) {
            int j = m - 1;
            
            while (j >= 0 && text[i] == pattern[j]) {
                --i;
                --j;
            }
            
            if (j < 0) {
                results.push_back(i + 1);
                
                i += m + 1;
            } else {
                auto it = badCharTable.find(text[i]);
                int shift = (it != badCharTable.end()) ? it->second : m;
                i += std::max(1, shift);
            }
        }
        
        return results;
    }
    
    // Структура для результата поиска
    struct SearchResult {
        int position;
        bool isFound;
        std::string foundPattern;
    };
    
    // Поиск с возвратом структуры результата
    static SearchResult Search(const std::string& text, const std::string& pattern) {
        int pos = SearchFirst(text, pattern);
        
        SearchResult result;
        result.position = pos;
        result.isFound = (pos != -1);
        result.foundPattern = result.isFound ? text.substr(pos, pattern.length()) : "";
        
        return result;
    }
};
