#pragma once
#include "Sequence.h"
#include "ArraySequence.h"
#include <utility> 

template <class T, class R>
Sequence<R>* map(const Sequence<T>* seq, R (*f)(const T&)) {
    Sequence<R>* result = new ArraySequence<R>();
    for (int i = 0; i < seq->GetLength(); i++) {
        R val = f(seq->Get(i));
        result->Append(val);
    }
    return result;
}

template <class T>
Sequence<T>* where(const Sequence<T>* seq, bool (*predicate)(const T&)) {
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
    T accum = startVal;
    for (int i = 0; i < seq->GetLength(); i++) {
        accum = f(seq->Get(i), accum);
    }
    return accum;
}

template <class T1, class T2>
Sequence<std::pair<T1, T2>>* zip(const Sequence<T1>* s1, const Sequence<T2>* s2) {
    int minLen = (s1->GetLength() < s2->GetLength()) 
               ? s1->GetLength() 
               : s2->GetLength();
    auto* result = new ArraySequence<std::pair<T1, T2>>();
    for (int i = 0; i < minLen; i++) {
        result->Append(std::make_pair(s1->Get(i), s2->Get(i)));
    }
    return result;
}

template <class T1, class T2>
std::pair<Sequence<T1>*, Sequence<T2>*> unzip(const Sequence<std::pair<T1, T2>>* seq) {
    auto* seq1 = new ArraySequence<T1>();
    auto* seq2 = new ArraySequence<T2>();
    for (int i = 0; i < seq->GetLength(); i++) {
        auto p = seq->Get(i);
        seq1->Append(p.first);
        seq2->Append(p.second);
    }
    return {seq1, seq2};
}

template <typename T1, typename T2>
void print(const Sequence<std::pair<T1, T2>>* seq) {
    std::cout << "[ ";
    if (seq->GetLength() != 0) {
        std::cout << "(" << seq->Get(0).first << ", " << seq->Get(0).second << ")";
    }
    for (int i = 1; i < seq->GetLength(); i++) {
        std::cout << ", " <<"(" << seq->Get(i).first << ", " << seq->Get(i).second << ")";
    }
    std::cout << " ]\n";
    return;
}

template <class T>
void print(const Sequence<T>* seq) {
    std::cout << "[ ";
    if (seq->GetLength() != 0) {
        std::cout << seq->Get(0);
    }
    for (int i = 1; i < seq->GetLength(); i++) {
        std::cout << ", " <<seq->Get(i);
    }
    std::cout << " ] \n";
    return;
}