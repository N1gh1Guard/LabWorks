#pragma once
#include "Sequence.h"
#include "LinkedList.h"
#include "Exception.h"

template <class T>
class ImmutableListSequence : public Sequence<T> {
private:
    LinkedList<T>* list;

    void copyFrom(const LinkedList<T>& source) {
        list = new LinkedList<T>(source);
    }

    void createFromArray(T* items, int count) {
        list = new LinkedList<T>(items, count);
    }

public:
    ImmutableListSequence() : list(new LinkedList<T>()) {}

    ImmutableListSequence(T* items, int count) {
        createFromArray(items, count);
    }

    ImmutableListSequence(const ImmutableListSequence<T>& other) {
        copyFrom(*other.list);
    }

    ImmutableListSequence(const Sequence<T>& other) {
        list = new LinkedList<T>();
        for (int i = 0; i < other.GetLength(); i++) {
            list->Append(other.Get(i));
        }
    }

    virtual ~ImmutableListSequence() {
        delete list;
    }

    ImmutableListSequence<T>& operator=(const ImmutableListSequence<T>& other) {
        if (this != &other) {
            delete list;
            copyFrom(*other.list);
        }
        return *this;
    }

    virtual T GetFirst() const override {
        return list->GetFirst();
    }

    virtual T GetLast() const override {
        return list->GetLast();
    }

    virtual T Get(int index) const override {
        return list->Get(index);
    }

    virtual int GetLength() const override {
        return list->GetLength();
    }

    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        LinkedList<T>* subList = list->GetSubList(startIndex, endIndex);
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>();
        delete result->list;
        result->list = subList;
        return result;
    }

    virtual Sequence<T>* Append(const T& item) override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        result->list->Append(item);
        return result;
    }

    virtual Sequence<T>* Prepend(const T& item) override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        result->list->Prepend(item);
        return result;
    }

    virtual Sequence<T>* InsertAt(const T& item, int index) override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        result->list->InsertAt(item, index);
        return result;
    }

    virtual Sequence<T>* Concat(const Sequence<T>* other) const override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        
        for (int i = 0; i < other->GetLength(); i++) {
            result->list->Append(other->Get(i));
        }
        
        return result;
    }

    virtual Sequence<T>* RemoveAt(int index) override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        result->list->RemoveAt(index);
        return result;
    }

    virtual const char* TypeName() const override {
        return "ImmutableListSequence";
    }

    virtual Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }

    bool IsEmpty() const {
        return list->GetLength() == 0;
    }

    bool operator==(const ImmutableListSequence<T>& other) const {
        if (GetLength() != other.GetLength()) return false;
        for (int i = 0; i < GetLength(); i++) {
            if (Get(i) != other.Get(i)) return false;
        }
        return true;
    }

    bool operator!=(const ImmutableListSequence<T>& other) const {
        return !(*this == other);
    }
};