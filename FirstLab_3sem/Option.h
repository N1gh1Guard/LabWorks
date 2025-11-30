#pragma once
#include "Exception.h"
#include "Error.h"

template <typename T>
class Option {
private:
    bool is_some;
    T value;

public:
    Option() : is_some(false) {}
    Option(T val) : is_some(true), value(val) {}
    
    static Option<T> Some(T val) { return Option(val); }
    static Option<T> None() { return Option(); }
    
    bool IsSome() const { return is_some; }
    bool IsNone() const { return !is_some; }
    
    T Unwrap() const {
        if (!is_some) {
            throw MyException(ErrorType::OptionError, static_cast<int>(OptionErrorCode::UnwrapOnNone));
        }
        return value;
    }
    
    T UnwrapOr(T default_val) const {
        return is_some ? value : default_val;
    }
    
    template <typename F>
    auto Bind(F f) const -> decltype(f(value)) {
        if (is_some) {
            return f(value);
        }
        return decltype(f(value))::None();
    }
    
    template <typename F>
    auto Map(F f) const -> Option<decltype(f(value))> {
        if (is_some) {
            return Option<decltype(f(value))>::Some(f(value));
        }
        return Option<decltype(f(value))>::None();
    }
    
    Option<T> AndThen(std::function<Option<T>(T)> f) const {
        if (is_some) {
            return f(value);
        }
        return None();
    }
};