#pragma once
#include "Sequence.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "Pair.h"
#include "Exception.h"

template <typename T>
int min_value(int a, int b) {
    return (a < b) ? a : b;
}

template <class T>
void print(const Sequence<T>* seq) {
    if (!seq) {
        throw MyException(4, "Null pointer passed to function");
    }
    std::cout << "[ ";
    for (int i = 0; i < seq->GetLength(); i++) {
        if (i > 0) std::cout << ", ";
        std::cout << seq->Get(i);
    }
    std::cout << " ]\n";
}

template <typename T1, typename T2>
Sequence<Pair<T1, T2>>* zip(const Sequence<T1>* s1, const Sequence<T2>* s2) {
    if (!s1 || !s2) {
        throw MyException(4, "Null pointer passed to function");
    }
    int minLen = min_value<T1>(s1->GetLength(), s2->GetLength());
    auto* result = new ArraySequence<Pair<T1, T2>>();
    for (int i = 0; i < minLen; i++) {
        result->Append(Pair<T1, T2>(s1->Get(i), s2->Get(i)));
    }
    return result;
}

template <typename T1, typename T2>
Pair<Sequence<T1>*, Sequence<T2>*> unzip(const Sequence<Pair<T1, T2>>* seq) {
    if (!seq) {
        throw MyException(4, "Null pointer passed to function");
    }
    auto* seq1 = new ArraySequence<T1>();
    auto* seq2 = new ArraySequence<T2>();
    for (int i = 0; i < seq->GetLength(); i++) {
        auto p = seq->Get(i);
        seq1->Append(p.first);
        seq2->Append(p.second);
    }
    Pair<Sequence<T1>*, Sequence<T2>*> result;
    result.first = seq1;
    result.second = seq2;
    return result;
}

template <class T, class R>
Sequence<R>* map(const Sequence<T>* seq, R (*f)(const T&)) {
    if (!seq) {
        throw MyException(4, "Null pointer passed to function");
    }
    Sequence<R>* result = new ArraySequence<R>();
    for (int i = 0; i < seq->GetLength(); i++) {
        R val = f(seq->Get(i));
        result->Append(val);
    }
    return result;
}

template <class T>
Sequence<T>* where(const Sequence<T>* seq, bool (*predicate)(const T&)) {
    if (!seq) {
        throw MyException(4, "Null pointer passed to function");
    }
    Sequence<T>* result = new ArraySequence<T>();
    for (int i = 0; i < seq->GetLength(); i++) {
        T elem = seq->Get(i);
        if (predicate(elem)) {
            result->Append(elem);
        }
    }
    return result;
}

template <class T>
T reduce(const Sequence<T>* seq, T (*f)(const T&, const T&), T startVal) {
    if (!seq) {
        throw MyException(4, "Null pointer passed to function");
    }
    T accum = startVal;
    for (int i = 0; i < seq->GetLength(); i++) {
        accum = f(seq->Get(i), accum);
    }
    return accum;
}