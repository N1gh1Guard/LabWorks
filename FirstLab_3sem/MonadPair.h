#pragma once
#include "Option.h"

template <typename T1, typename T2>
class MonadPair {
public:
    T1 first;
    T2 second;
    
    MonadPair(T1 a, T2 b) : first(a), second(b) {}
    
    template <typename F>
    auto Bind(F f) const -> decltype(f(second)) {
        return f(second);
    }
    
    template <typename F>
    auto Map(F f) const -> MonadPair<T1, decltype(f(second))> {
        return MonadPair<T1, decltype(f(second))>(first, f(second));
    }
    
    template <typename F>
    auto AndThen(F f) const -> decltype(f(second)) {
        return Bind(f);
    }
};