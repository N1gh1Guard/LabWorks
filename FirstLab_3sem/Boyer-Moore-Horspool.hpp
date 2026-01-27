#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>

class BoyerMooreHorspool {
private:
    static char normalizeChar(char c) {
        if (std::isdigit(static_cast<unsigned char>(c)) || 
            std::ispunct(static_cast<unsigned char>(c))) {
            return '\0';
        }
        return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    
    static std::string normalizeString(const std::string& str) {
        std::string result;
        for (char c : str) {
            char normalized = normalizeChar(c);
            if (normalized != '\0') {
                result += normalized;
            }
        }
        return result;
    }
    
    static std::vector<size_t> mapPositions(const std::string& original, const std::string& normalized) {
        std::vector<size_t> positions;
        size_t origIdx = 0;
        for (size_t i = 0; i < normalized.length(); ++i) {
            while (origIdx < original.length()) {
                char normalized = normalizeChar(original[origIdx]);
                if (normalized != '\0') {
                    positions.push_back(origIdx);
                    origIdx++;
                    break;
                }
                origIdx++;
            }
        }
        return positions;
    }
    
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
        if (pattern.empty()) {
            return -1;
        }
        
        std::string normText = normalizeString(text);
        std::string normPattern = normalizeString(pattern);
        
        if (normPattern.empty() || normText.length() < normPattern.length()) {
            return -1;
        }
        
        std::vector<size_t> positions = mapPositions(text, normText);
        
        auto badCharTable = BuildBadCharTable(normPattern);
        int n = static_cast<int>(normText.length());
        int m = static_cast<int>(normPattern.length());
        
        int i = m - 1;
        
        while (i < n) {
            int j = m - 1;
            
            while (j >= 0 && normText[i] == normPattern[j]) {
                --i;
                --j;
            }
            
            if (j < 0) {
                if (i + 1 >= 0 && i + 1 < static_cast<int>(positions.size())) {
                    return static_cast<int>(positions[i + 1]);
                }
                return -1;
            }
            
            auto it = badCharTable.find(normText[i]);
            int shift = (it != badCharTable.end()) ? it->second : m;
            
            i += std::max(1, shift);
        }
        
        return -1;
    }
    
    static std::vector<int> SearchAll(const std::string& text, const std::string& pattern) {
        std::vector<int> results;
        
        if (pattern.empty()) {
            return results;
        }
        
        std::string normText = normalizeString(text);
        std::string normPattern = normalizeString(pattern);
        
        if (normPattern.empty() || normText.length() < normPattern.length()) {
            return results;
        }
        
        auto badCharTable = BuildBadCharTable(normPattern);
        int n = static_cast<int>(normText.length());
        int m = static_cast<int>(normPattern.length());
        int i = m - 1;
        
        std::vector<size_t> positions = mapPositions(text, normText);
        
        while (i < n) {
            int j = m - 1;
            
            while (j >= 0 && normText[i] == normPattern[j]) {
                --i;
                --j;
            }
            
            if (j < 0) {
                if (i + 1 < static_cast<int>(positions.size())) {
                    results.push_back(static_cast<int>(positions[i + 1]));
                } else {
                    results.push_back(i + 1);
                }
                
                i += m + 1;
            } else {
                auto it = badCharTable.find(normText[i]);
                int shift = (it != badCharTable.end()) ? it->second : m;
                i += std::max(1, shift);
            }
        }
        
        return results;
    }
    
    struct SearchResult {
        int position;
        bool isFound;
        std::string foundPattern;
    };
    
    static SearchResult Search(const std::string& text, const std::string& pattern) {
        int pos = SearchFirst(text, pattern);
        
        SearchResult result;
        result.position = pos;
        result.isFound = (pos != -1);
        result.foundPattern = result.isFound ? text.substr(pos, pattern.length()) : "";
        
        return result;
    }
};
