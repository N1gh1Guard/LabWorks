#pragma once
#include "DynamicArray.h"
#include "Sequence.h"
#include <stdexcept>
#include "Training.h"

template <class T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* items;
    int count;
    //убрать
public:
    ArraySequence() {
        items = new DynamicArray<T>(4);
        count = 0;
    }

    ArraySequence(const T* arr, int length) {
        if (length < 0) {
            throw MyException(ErrorType::NegativeSize, 0);
        }
        items = new DynamicArray<T>(length);
        for (int i = 0; i < length; i++) {
            items->Set(i, arr[i]);
        }
        count = length;
    }

    ArraySequence(const ArraySequence<T>& other) {
        count = other.count;
        items = new DynamicArray<T>(*other.items);
    }

    virtual ~ArraySequence() {
        delete items;
    }

    virtual T GetFirst() const override {
        if (items->GetSize() == 0) {
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
        if (count == items->Size()) {
            items->Resize(items->Size() * 2);
        }
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
        if (count == items->Size()) {
            items->Resize(items->Size() * 2);
        }
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
        if (index >= count) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        if (count == items->Size()) {
            items->Resize(items->Size() * 2);
        }
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

    void smartReverse() {
        reverse();
    }
    //абсолютно ненужно

    virtual Sequence<T>* Clone() const override {
        return new ArraySequence<T>(*this);
    }
};