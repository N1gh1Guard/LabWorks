#pragma once
#include "Option.h"
#include <functional>

template <typename T1, typename T2>
class MonadPair {
public:
    T1 first;
    T2 second;
    
    MonadPair(T1 a, T2 b) : first(a), second(b) {}
    
    // Монадическая операция bind
    template <typename F>
    auto Bind(F f) const -> decltype(f(second)) {
        return f(second);
    }
    
    // Монадическая операция map
    template <typename F>
    auto Map(F f) const -> MonadPair<T1, decltype(f(second))> {
        return MonadPair<T1, decltype(f(second))>(first, f(second));
    }
    
    // Комбинированная операция
    template <typename F>
    auto AndThen(F f) const -> decltype(f(second)) {
        return Bind(f);
    }
    
    // Преобразование в std::pair
    std::pair<T1, T2> ToStdPair() const {
        return std::make_pair(first, second);
    }
};

// Специализация для Option
template <typename T1, typename T2>
class MonadPair<Option<T1>, Option<T2>> {
public:
    Option<T1> first;
    Option<T2> second;
    
    MonadPair(Option<T1> a, Option<T2> b) : first(a), second(b) {}
    
    bool IsBothSome() const {
        return first.IsSome() && second.IsSome();
    }
    
    bool IsAnyNone() const {
        return first.IsNone() || second.IsNone();
    }
    
    // Монадическая операция bind
    template <typename F>
    auto Bind(F f) const -> decltype(f(first.Unwrap(), second.Unwrap())) {
        if (IsBothSome()) {
            return f(first.Unwrap(), second.Unwrap());
        }
        throw MyException(ErrorType::OptionError, 
                          static_cast<int>(OptionErrorCode::InvalidMapping));
    }
    
    // Монадическая операция map
    template <typename F>
    auto Map(F f) const -> Option<decltype(f(first.Unwrap(), second.Unwrap()))> {
        if (IsBothSome()) {
            return Option<decltype(f(first.Unwrap(), second.Unwrap()))>::Some(
                f(first.Unwrap(), second.Unwrap()));
        }
        return Option<decltype(f(first.Unwrap(), second.Unwrap()))>::None();
    }
};