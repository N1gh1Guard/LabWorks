#pragma once

template <typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;
    
    Pair() : first(T1()), second(T2()) {}
    
    Pair(T1 f, T2 s) : first(f), second(s) {}
    
    bool operator==(const Pair& other) const {
        return first == other.first && second == other.second;
    }
    
    bool operator!=(const Pair& other) const {
        return !(*this == other);
    }
};