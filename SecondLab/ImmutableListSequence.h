#pragma once
#include "ListSequence.h"

template <class T>
class ImmutableListSequence : public ListSequence<T> {
public:
    ImmutableListSequence()
        : ListSequence<T>() {}

    ImmutableListSequence(T* arr, int count)
        : ListSequence<T>(arr, count) {}

    ImmutableListSequence(const ImmutableListSequence<T>& other)
        : ListSequence<T>(other) {}

    virtual ~ImmutableListSequence() {}

    virtual Sequence<T>* Append(const T& item) override {
        auto* newSeq = new ImmutableListSequence<T>(*this);
        newSeq->ListSequence<T>::Append(item);
        return newSeq;
    }

    virtual Sequence<T>* Prepend(const T& item) override {
        auto* newSeq = new ImmutableListSequence<T>(*this);
        newSeq->ListSequence<T>::Prepend(item);
        return newSeq;
    }

    virtual Sequence<T>* InsertAt(const T& item, int index) override {
        auto* newSeq = new ImmutableListSequence<T>(*this);
        newSeq->ListSequence<T>::InsertAt(item, index);
        return newSeq;
    }

    virtual Sequence<T>* Concat(const Sequence<T>* seq) const override {
        auto* newSeq = new ImmutableListSequence<T>(*this);
        newSeq->ListSequence<T>::Concat(seq);
        return newSeq;
    }

    virtual Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }
};