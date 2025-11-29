#pragma once
#include "Sequence.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "Option.h"
#include "MonadPair.h"
#include "MonadTuple.h"
#include <tuple>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <iostream>

// Вспомогательная функция для вычисления минимальной длины
template <typename... Seqs>
size_t min_length(const Seqs*... seqs) {
    std::vector<size_t> lengths = {static_cast<size_t>(seqs->GetLength())...};
    return *std::min_element(lengths.begin(), lengths.end());
}

// Шаблонная функция для zip произвольного количества последовательностей
template <typename... Ts>
Sequence<MonadTuple<Ts...>>* zip_as_tuple(const Sequence<Ts>*... seqs) {
    const size_t minLen = min_length(seqs...);
    auto* result = new ArraySequence<MonadTuple<Ts...>>();
    
    for (size_t i = 0; i < minLen; i++) {
        result->Append(MonadTuple<Ts...>(seqs->Get(i)...));
    }
    return result;
}

// Шаблонная функция для unzip кортежа
template <typename... Ts>
std::tuple<Sequence<Ts>*...> unzip_tuple(const Sequence<MonadTuple<Ts...>>* seq) {
    std::tuple<Sequence<Ts>*...> sequences = std::make_tuple(new ArraySequence<Ts>()...);
    
    for (int i = 0; i < seq->GetLength(); i++) {
        auto tuple = seq->Get(i);
        auto stdTuple = tuple.ToStdTuple();
        
        size_t index = 0;
        auto append_to_seq = [&](auto& value) {
            std::get<index>(sequences)->Append(value);
            index++;
        };
        
        std::apply([&](auto&... values) {
            (append_to_seq(values), ...);
        }, stdTuple);
    }
    return sequences;
}

// Функция для вывода кортежа
template <typename... Ts>
void print(const Sequence<MonadTuple<Ts...>>* seq) {
    std::cout << "[ ";
    for (int i = 0; i < seq->GetLength(); i++) {
        if (i > 0) std::cout << ", ";
        auto tuple = seq->Get(i).ToStdTuple();
        std::cout << "(";
        bool first = true;
        std::apply([&](const auto&... values) {
            ((std::cout << (first ? "" : ", ") << values, first = false), ...);
        }, tuple);
        std::cout << ")";
    }
    std::cout << " ]\n";
}

// Остальные функции остаются без изменений
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
Sequence<MonadPair<T1, T2>>* zip(const Sequence<T1>* s1, const Sequence<T2>* s2) {
    int minLen = (s1->GetLength() < s2->GetLength()) 
               ? s1->GetLength() 
               : s2->GetLength();
    auto* result = new ArraySequence<MonadPair<T1, T2>>();
    for (int i = 0; i < minLen; i++) {
        result->Append(MonadPair<T1, T2>(s1->Get(i), s2->Get(i)));
    }
    return result;
}

template <class T1, class T2>
std::pair<Sequence<T1>*, Sequence<T2>*> unzip(const Sequence<MonadPair<T1, T2>>* seq) {
    auto* seq1 = new ArraySequence<T1>();
    auto* seq2 = new ArraySequence<T2>();
    for (int i = 0; i < seq->GetLength(); i++) {
        auto p = seq->Get(i);
        seq1->Append(p.first);
        seq2->Append(p.second);
    }
    return {seq1, seq2};
}