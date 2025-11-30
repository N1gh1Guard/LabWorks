#pragma once
#include "Sequence.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "Option.h"
#include "MonadPair.h"
#include "MonadTuple.h"
#include "BasicStructures.h"

template <typename T>
int min_value(int a, int b) {
    return (a < b) ? a : b;
}

template <class T>
void print(const Sequence<T>* seq) {
    std::cout << "[ ";
    for (int i = 0; i < seq->GetLength(); i++) {
        if (i > 0) std::cout << ", ";
        std::cout << seq->Get(i);
    }
    std::cout << " ]\n";
}

template <typename T1, typename T2>
Sequence<MonadPair<T1, T2>>* zip(const Sequence<T1>* s1, const Sequence<T2>* s2) {
    int minLen = min_value<T1>(s1->GetLength(), s2->GetLength());
    auto* result = new ArraySequence<MonadPair<T1, T2>>();
    for (int i = 0; i < minLen; i++) {
        result->Append(MonadPair<T1, T2>(s1->Get(i), s2->Get(i)));
    }
    return result;
}

template <typename T1, typename T2>
Pair<Sequence<T1>*, Sequence<T2>*> unzip(const Sequence<MonadPair<T1, T2>>* seq) {
    auto* seq1 = new ArraySequence<T1>();
    auto* seq2 = new ArraySequence<T2>();
    for (int i = 0; i < seq->GetLength(); i++) {
        auto p = seq->Get(i);
        seq1->Append(p.first);
        seq2->Append(p.second);
    }
    return Pair<Sequence<T1>*, Sequence<T2>*>(seq1, seq2);
}

template <typename T1, typename T2>
Sequence<MonadTuple2<T1, T2>>* zip_as_tuple(Sequence<T1>* s1, Sequence<T2>* s2) {
    int minLen = min_value<T1>(s1->GetLength(), s2->GetLength());
    auto* result = new ArraySequence<MonadTuple2<T1, T2>>();
    for (int i = 0; i < minLen; i++) {
        result->Append(MonadTuple2<T1, T2>(s1->Get(i), s2->Get(i)));
    }
    return result;
}

template <typename T1, typename T2, typename T3>
Sequence<MonadTuple3<T1, T2, T3>>* zip_as_tuple(Sequence<T1>* s1, Sequence<T2>* s2, Sequence<T3>* s3) {
    int len1 = s1->GetLength();
    int len2 = s2->GetLength();
    int len3 = s3->GetLength();
    int minLen = min_value<T1>(len1, min_value<T2>(len2, len3));
    
    auto* result = new ArraySequence<MonadTuple3<T1, T2, T3>>();
    for (int i = 0; i < minLen; i++) {
        result->Append(MonadTuple3<T1, T2, T3>(s1->Get(i), s2->Get(i), s3->Get(i)));
    }
    return result;
}

template <typename T1, typename T2, typename T3, typename T4>
Sequence<MonadTuple4<T1, T2, T3, T4>>* zip_as_tuple(Sequence<T1>* s1, Sequence<T2>* s2, Sequence<T3>* s3, Sequence<T4>* s4) {
    int len1 = s1->GetLength();
    int len2 = s2->GetLength();
    int len3 = s3->GetLength();
    int len4 = s4->GetLength();
    int minLen = min_value<T1>(len1, min_value<T2>(len2, min_value<T3>(len3, len4)));
    
    auto* result = new ArraySequence<MonadTuple4<T1, T2, T3, T4>>();
    for (int i = 0; i < minLen; i++) {
        result->Append(MonadTuple4<T1, T2, T3, T4>(s1->Get(i), s2->Get(i), s3->Get(i), s4->Get(i)));
    }
    return result;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
Sequence<MonadTuple5<T1, T2, T3, T4, T5>>* zip_as_tuple(Sequence<T1>* s1, Sequence<T2>* s2, Sequence<T3>* s3, Sequence<T4>* s4, Sequence<T5>* s5) {
    int len1 = s1->GetLength();
    int len2 = s2->GetLength();
    int len3 = s3->GetLength();
    int len4 = s4->GetLength();
    int len5 = s5->GetLength();
    int minLen = min_value<T1>(len1, min_value<T2>(len2, min_value<T3>(len3, min_value<T4>(len4, len5))));
    
    auto* result = new ArraySequence<MonadTuple5<T1, T2, T3, T4, T5>>();
    for (int i = 0; i < minLen; i++) {
        result->Append(MonadTuple5<T1, T2, T3, T4, T5>(s1->Get(i), s2->Get(i), s3->Get(i), s4->Get(i), s5->Get(i)));
    }
    return result;
}

template <typename T1, typename T2>
Pair<Sequence<T1>*, Sequence<T2>*> unzip_tuple(const Sequence<MonadTuple2<T1, T2>>* seq) {
    auto* seq1 = new ArraySequence<T1>();
    auto* seq2 = new ArraySequence<T2>();
    for (int i = 0; i < seq->GetLength(); i++) {
        auto tuple = seq->Get(i);
        seq1->Append(tuple.first);
        seq2->Append(tuple.second);
    }
    return Pair<Sequence<T1>*, Sequence<T2>*>(seq1, seq2);
}

template <typename T1, typename T2, typename T3>
Triple<Sequence<T1>*, Sequence<T2>*, Sequence<T3>*> unzip_tuple(const Sequence<MonadTuple3<T1, T2, T3>>* seq) {
    auto* seq1 = new ArraySequence<T1>();
    auto* seq2 = new ArraySequence<T2>();
    auto* seq3 = new ArraySequence<T3>();
    for (int i = 0; i < seq->GetLength(); i++) {
        auto tuple = seq->Get(i);
        seq1->Append(tuple.first);
        seq2->Append(tuple.second);
        seq3->Append(tuple.third);
    }
    return Triple<Sequence<T1>*, Sequence<T2>*, Sequence<T3>*>(seq1, seq2, seq3);
}

template <typename T1, typename T2, typename T3, typename T4>
Quadruple<Sequence<T1>*, Sequence<T2>*, Sequence<T3>*, Sequence<T4>*> unzip_tuple(const Sequence<MonadTuple4<T1, T2, T3, T4>>* seq) {
    auto* seq1 = new ArraySequence<T1>();
    auto* seq2 = new ArraySequence<T2>();
    auto* seq3 = new ArraySequence<T3>();
    auto* seq4 = new ArraySequence<T4>();
    for (int i = 0; i < seq->GetLength(); i++) {
        auto tuple = seq->Get(i);
        seq1->Append(tuple.first);
        seq2->Append(tuple.second);
        seq3->Append(tuple.third);
        seq4->Append(tuple.fourth);
    }
    return Quadruple<Sequence<T1>*, Sequence<T2>*, Sequence<T3>*, Sequence<T4>*>(seq1, seq2, seq3, seq4);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
Quintuple<Sequence<T1>*, Sequence<T2>*, Sequence<T3>*, Sequence<T4>*, Sequence<T5>*> unzip_tuple(const Sequence<MonadTuple5<T1, T2, T3, T4, T5>>* seq) {
    auto* seq1 = new ArraySequence<T1>();
    auto* seq2 = new ArraySequence<T2>();
    auto* seq3 = new ArraySequence<T3>();
    auto* seq4 = new ArraySequence<T4>();
    auto* seq5 = new ArraySequence<T5>();
    for (int i = 0; i < seq->GetLength(); i++) {
        auto tuple = seq->Get(i);
        seq1->Append(tuple.first);
        seq2->Append(tuple.second);
        seq3->Append(tuple.third);
        seq4->Append(tuple.fourth);
        seq5->Append(tuple.fifth);
    }
    return Quintuple<Sequence<T1>*, Sequence<T2>*, Sequence<T3>*, Sequence<T4>*, Sequence<T5>*>(seq1, seq2, seq3, seq4, seq5);
}

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