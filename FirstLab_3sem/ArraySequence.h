#pragma once
#include "DynamicArray.h"
#include "Sequence.h"
#include <stdexcept>
#include "Exception.h"

template <class T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* items;
    int count;
    int capacity;

    void resizeIfNeeded() {
        if (count >= capacity) {
            int newCapacity = (capacity == 0) ? 4 : capacity * 2;
            DynamicArray<T>* newItems = new DynamicArray<T>(newCapacity);
            for (int i = 0; i < count; i++) {
                newItems->Set(i, items->Get(i));
            }
            delete items;
            items = newItems;
            capacity = newCapacity;
        }
    }

public:
    ArraySequence() {
        capacity = 4;
        items = new DynamicArray<T>(capacity);
        count = 0;
    }

    ArraySequence(const T* arr, int length) {
        if (length < 0) {
            throw MyException(ErrorType::NegativeSize, 0);
        }
        capacity = (length == 0) ? 4 : length * 2;
        items = new DynamicArray<T>(capacity);
        for (int i = 0; i < length; i++) {
            items->Set(i, arr[i]);
        }
        count = length;
    }

    ArraySequence(const ArraySequence<T>& other) {
        capacity = other.capacity;
        count = other.count;
        items = new DynamicArray<T>(*other.items);
    }

    virtual ~ArraySequence() {
        delete items;
    }

    virtual T GetFirst() const override {
        if (count == 0) {
            throw MyException(ErrorType::OutOfRange, 2);
        }
        return items->Get(0);
    }

    virtual T GetLast() const override {
        if (count == 0) {
            throw MyException(ErrorType::OutOfRange, 2);
        }
        return items->Get(count - 1);
    }

    virtual T Get(int index) const override {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (index >= count) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        return items->Get(index);
    }

    virtual int GetLength() const override {
        return count;
    }

    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (endIndex < 0 || startIndex > endIndex || endIndex >= count) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        int newLen = endIndex - startIndex + 1;
        T* temp = new T[newLen];
        for (int i = 0; i < newLen; i++) {
            temp[i] = items->Get(startIndex + i);
        }
        ArraySequence<T>* result = new ArraySequence<T>(temp, newLen);
        delete[] temp;
        return result;
    }

    virtual Sequence<T>* Append(const T& item) override {
        resizeIfNeeded();
        items->Set(count, item);
        count++;
        return this;
    }

    virtual const char* TypeName() const override {
        return "ArraySequence";
    }

    virtual Sequence<T>* RemoveAt(int index) override {
        if (index < 0 || index >= count) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        for (int i = index; i < count - 1; i++) {
            items->Set(i, items->Get(i + 1));
        }
        count--;
        return this;
    }

    virtual Sequence<T>* Prepend(const T& item) override {
        resizeIfNeeded();
        for (int i = count; i > 0; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(0, item);
        count++;
        return this;
    }

    virtual Sequence<T>* InsertAt(const T& item, int index) override {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (index > count) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        
        resizeIfNeeded();
        for (int i = count; i > index; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
        count++;
        return this;
    }

    virtual Sequence<T>* Concat(const Sequence<T>* seq) const override {
        ArraySequence<T>* newSeq = new ArraySequence<T>(*this);
        for (int i = 0; i < seq->GetLength(); i++) {
            newSeq->Append(seq->Get(i));
        }
        return newSeq;
    }

    void reverse() {
        int n = count;
        for (int i = 0; i < n / 2; ++i) {
            T tmp = items->Get(i);
            items->Set(i, items->Get(n - 1 - i));
            items->Set(n - 1 - i, tmp);
        }
    }

    virtual Sequence<T>* Clone() const override {
        return new ArraySequence<T>(*this);
    }

    ArraySequence<T>& operator=(const ArraySequence<T>& other) {
        if (this != &other) {
            delete items;
            capacity = other.capacity;
            count = other.count;
            items = new DynamicArray<T>(*other.items);
        }
        return *this;
    }

    bool operator==(const ArraySequence<T>& other) const {
        if (count != other.count) return false;
        for (int i = 0; i < count; i++) {
            if (items->Get(i) != other.items->Get(i)) return false;
        }
        return true;
    }

    bool operator!=(const ArraySequence<T>& other) const {
        return !(*this == other);
    }
};