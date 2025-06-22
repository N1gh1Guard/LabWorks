#pragma once

#include "Error.h"

template <typename T>
class Option {
private:
    bool hasValue;
    T value;
public:
    Option() : hasValue(false) {}
    Option(const T& val) : hasValue(true), value(val) {}

    static Option<T> Some(const T& val) { return Option<T>(val); }
    static Option<T> None() { return Option<T>(); }

    bool IsSome() const { return hasValue; }
    bool IsNone() const { return !hasValue; }

    T Unwrap() const {
        if (!hasValue) throw Errors[RUNTIME_ERROR].message;
        return value;
    }
    T UnwrapOr(const T& defaultVal) const {
        return hasValue ? value : defaultVal;
    }
};