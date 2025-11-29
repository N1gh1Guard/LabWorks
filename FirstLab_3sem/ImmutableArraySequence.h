#pragma once
#include "ArraySequence.h"

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence()
        : ArraySequence<T>() {}

    ImmutableArraySequence(T* arr, int count)
        : ArraySequence<T>(arr, count) {}

    ImmutableArraySequence(const ImmutableArraySequence<T>& other)
        : ArraySequence<T>(other) {}

    virtual ~ImmutableArraySequence() {}

    virtual Sequence<T>* Append(const T& item) override {
        ImmutableArraySequence<T>* newSeq = new ImmutableArraySequence<T>(*this);
        newSeq->ArraySequence<T>::Append(item);
        return newSeq;
    }

    virtual Sequence<T>* Prepend(const T& item) override {
        ImmutableArraySequence<T>* newSeq = new ImmutableArraySequence<T>(*this);
        newSeq->ArraySequence<T>::Prepend(item);
        return newSeq;
    }

    virtual Sequence<T>* InsertAt(const T& item, int index) override {
        ImmutableArraySequence<T>* newSeq = new ImmutableArraySequence<T>(*this);
        newSeq->ArraySequence<T>::InsertAt(item, index);
        return newSeq;
    }

    virtual Sequence<T>* Concat(const Sequence<T>* seq) const override {
        auto* newSeq = new ImmutableArraySequence<T>(*this);
        newSeq->ArraySequence<T>::Concat(seq);
        return newSeq;
    }

    virtual Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }
};