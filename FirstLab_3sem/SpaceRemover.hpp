#pragma once
#include <string>
#include <cctype>

class SpaceRemover {
public:
    enum class State {
        Normal,
        SingleSpace,
        MultipleSpaces
    };
    
    static std::string RemoveMultipleSpaces(const std::string& text) {
        std::string result;
        result.reserve(text.length());
        
        State currentState = State::Normal;
        
        for (char c : text) {
            bool isSpace = std::isspace(static_cast<unsigned char>(c));
            
            switch (currentState) {
                case State::Normal:
                    if (isSpace) {
                        result += c;
                        currentState = State::SingleSpace;
                    } else {
                        result += c;
                    }
                    break;
                    
                case State::SingleSpace:
                    if (isSpace) {
                        currentState = State::MultipleSpaces;
                    } else {
                        result += c;
                        currentState = State::Normal;
                    }
                    break;
                    
                case State::MultipleSpaces:
                    if (!isSpace) {
                        result += c;
                        currentState = State::Normal;
                    }
                    break;
            }
        }
        
        return result;
    }
    
    static std::string RemoveMultipleSpacesDetailed(const std::string& text) {
        std::string result;
        result.reserve(text.length());
        
        State currentState = State::Normal;
        int spaceCount = 0;
        int removedSpaces = 0;
        
        for (char c : text) {
            bool isSpace = std::isspace(static_cast<unsigned char>(c));
            
            switch (currentState) {
                case State::Normal:
                    if (isSpace) {
                        result += c;
                        currentState = State::SingleSpace;
                        spaceCount = 1;
                    } else {
                        result += c;
                    }
                    break;
                    
                case State::SingleSpace:
                    if (isSpace) {
                        currentState = State::MultipleSpaces;
                        spaceCount++;
                        removedSpaces++;
                    } else {
                        result += c;
                        currentState = State::Normal;
                        spaceCount = 0;
                    }
                    break;
                    
                case State::MultipleSpaces:
                    if (isSpace) {
                        spaceCount++;
                        removedSpaces++;
                    } else {
                        result += c;
                        currentState = State::Normal;
                        spaceCount = 0;
                    }
                    break;
            }
        }
        
        return result;
    }
    
    struct ProcessingStats {
        size_t originalLength;
        size_t resultLength;
        size_t spacesRemoved;
        size_t spacesPreserved;
    };
    
    static ProcessingStats ProcessWithStats(const std::string& text) {
        ProcessingStats stats;
        stats.originalLength = text.length();
        
        std::string result;
        result.reserve(text.length());
        
        State currentState = State::Normal;
        size_t spacesRemoved = 0;
        size_t spacesPreserved = 0;
        
        for (char c : text) {
            bool isSpace = std::isspace(static_cast<unsigned char>(c));
            
            switch (currentState) {
                case State::Normal:
                    if (isSpace) {
                        result += c;
                        currentState = State::SingleSpace;
                        spacesPreserved++;
                    } else {
                        result += c;
                    }
                    break;
                    
                case State::SingleSpace:
                    if (isSpace) {
                        currentState = State::MultipleSpaces;
                        spacesRemoved++;
                    } else {
                        result += c;
                        currentState = State::Normal;
                    }
                    break;
                    
                case State::MultipleSpaces:
                    if (isSpace) {
                        spacesRemoved++;
                    } else {
                        result += c;
                        currentState = State::Normal;
                    }
                    break;
            }
        }
        
        stats.resultLength = result.length();
        stats.spacesRemoved = spacesRemoved;
        stats.spacesPreserved = spacesPreserved;
        
        return stats;
    }
    
    static std::string GetStateName(State state) {
        switch (state) {
            case State::Normal:
                return "Normal";
            case State::SingleSpace:
                return "SingleSpace";
            case State::MultipleSpaces:
                return "MultipleSpaces";
        }
        return "Unknown";
    }
};
