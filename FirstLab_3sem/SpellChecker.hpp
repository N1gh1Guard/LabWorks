#pragma once
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <cctype>
#include <map>

#if __has_include(<QString>)
#include <QString>
#elif __has_include(<QtCore/QString>)
#include <QtCore/QString>
#elif __has_include(<QtCore/qstring.h>)
#include <QtCore/qstring.h>
#endif

class SpellChecker {
private:
    std::set<std::string> dictionary;
    mutable std::map<std::pair<std::string, std::string>, int> distanceCache;
    
    static std::string ToLower(const std::string& str) {
#ifdef QT_CORE_LIB
        QString qstr = QString::fromUtf8(str.c_str());
        QString lower = qstr.toLower();
        return lower.toUtf8().toStdString();
#else
        std::string result = str;
        for (char &c : result) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        return result;
#endif
    }
    
    static bool IsWordChar(char c) {
        unsigned char uc = static_cast<unsigned char>(c);
        return !std::isspace(uc) && !std::ispunct(uc) && !std::isdigit(uc);
    }
    
public:
    int LevenshteinDistance(const std::string& s1, const std::string& s2) const {
        auto cacheKey = std::make_pair(s1, s2);
        auto reverseCacheKey = std::make_pair(s2, s1);
        
        auto cacheIt = distanceCache.find(cacheKey);
        if (cacheIt != distanceCache.end()) {
            return cacheIt->second;
        }
        
        cacheIt = distanceCache.find(reverseCacheKey);
        if (cacheIt != distanceCache.end()) {
            return cacheIt->second;
        }
        
#ifdef QT_CORE_LIB
        QString qs1 = QString::fromUtf8(s1.c_str()).toLower();
        QString qs2 = QString::fromUtf8(s2.c_str()).toLower();
        const int m = qs1.length();
        const int n = qs2.length();
        
        if (m == 0) return n;
        if (n == 0) return m;
        
        std::vector<int> prev(n + 1);
        std::vector<int> curr(n + 1);
        
        for (int j = 0; j <= n; ++j) {
            prev[j] = j;
        }
        
        for (int i = 1; i <= m; ++i) {
            curr[0] = i;
            
            for (int j = 1; j <= n; ++j) {
                QChar c1 = qs1[i - 1];
                QChar c2 = qs2[j - 1];
                int cost = (c1 == c2) ? 0 : 1;
                
                int del = prev[j] + 1;
                int ins = curr[j - 1] + 1;
                int sub = prev[j - 1] + cost;
                
                curr[j] = std::min(del, std::min(ins, sub));
            }
            
            std::swap(prev, curr);
        }
        
        int result = prev[n];
#else
        const size_t m = s1.length();
        const size_t n = s2.length();
        
        if (m == 0) return static_cast<int>(n);
        if (n == 0) return static_cast<int>(m);
        
        std::vector<int> prev(n + 1);
        std::vector<int> curr(n + 1);
        
        for (size_t j = 0; j <= n; ++j) {
            prev[j] = static_cast<int>(j);
        }
        
        for (size_t i = 1; i <= m; ++i) {
            curr[0] = static_cast<int>(i);
            
            for (size_t j = 1; j <= n; ++j) {
                char c1 = static_cast<char>(std::tolower(static_cast<unsigned char>(s1[i - 1])));
                char c2 = static_cast<char>(std::tolower(static_cast<unsigned char>(s2[j - 1])));
                int cost = (c1 == c2) ? 0 : 1;
                
                int del = prev[j] + 1;
                int ins = curr[j - 1] + 1;
                int sub = prev[j - 1] + cost;
                
                curr[j] = std::min(del, std::min(ins, sub));
            }
            
            std::swap(prev, curr);
        }
        
        int result = prev[n];
#endif
        distanceCache[cacheKey] = result;
        return result;
    }
    
    void LoadDictionary(const std::set<std::string>& dict) {
        dictionary.clear();
        for (const auto& word : dict) {
            dictionary.insert(ToLower(word));
        }
        distanceCache.clear();
    }
    
    void AddWord(const std::string& word) {
        dictionary.insert(ToLower(word));
    }
    
    bool IsCorrect(const std::string& word) const {
        return dictionary.find(ToLower(word)) != dictionary.end();
    }
    
    struct WordCheckResult {
        bool isCorrect;
        std::string word;
        std::vector<std::string> suggestions;
    };
    
    WordCheckResult CheckWord(const std::string& word, int maxDistance = 2) const {
        WordCheckResult result{false, word, {}};
        result.isCorrect = IsCorrect(word);
        
        if (result.isCorrect) {
            return result;
        }
        
        std::string lowerWord = ToLower(word);
        std::vector<std::pair<int, std::string>> candidates;
        
        for (const auto& dictWord : dictionary) {
            int distance = LevenshteinDistance(lowerWord, dictWord);
            
            if (distance <= maxDistance && distance > 0) {
                candidates.push_back(std::make_pair(distance, dictWord));
            }
        }
        
        std::sort(candidates.begin(), candidates.end());
        
        size_t limit = (candidates.size() < 5) ? candidates.size() : 5;
        for (size_t i = 0; i < limit; ++i) {
            result.suggestions.push_back(candidates[i].second);
        }
        
        return result;
    }
    
    struct TextCheckResult {
        std::vector<WordCheckResult> results;
        int totalWords;
        int errorCount;
        double errorPercentage;
    };
    
    TextCheckResult CheckText(const std::string& text) const {
        TextCheckResult result{std::vector<WordCheckResult>(), 0, 0, 0.0};
        std::string word;

        auto flushWord = [&]() {
            if (word.empty()) return;
            WordCheckResult checkResult{IsCorrect(word), word, {}};
            result.results.push_back(checkResult);
            result.totalWords++;
            if (!checkResult.isCorrect) {
                result.errorCount++;
            }
            word.clear();
        };

        for (size_t i = 0; i < text.length(); ++i) {
            char c = text[i];
            if (IsWordChar(c)) {
                word += c;
            } else {
                flushWord();
            }
        }
        flushWord();
        
        result.errorPercentage = (result.totalWords > 0) 
            ? (100.0 * result.errorCount / result.totalWords)
            : 0.0;
        
        return result;
    }
    
    size_t GetCacheSize() const {
        return distanceCache.size();
    }
    
    void ClearCache() const {
        distanceCache.clear();
    }
    
    size_t GetDictionarySize() const {
        return dictionary.size();
    }
};