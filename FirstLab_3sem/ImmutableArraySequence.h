#pragma once
#include "Sequence.h"
#include "DynamicArray.h"
#include "Exception.h"

template <class T>
class ImmutableArraySequence : public Sequence<T> {
private:
    T* data;
    int count;

    ImmutableArraySequence(const Sequence<T>& other) {
        count = other.GetLength();
        if (count > 0) {
            data = new T[count];
            for (int i = 0; i < count; i++) {
                data[i] = other.Get(i);
            }
        } else {
            data = nullptr;
        }
    }

    void copyFrom(const T* source, int sourceCount) {
        if (sourceCount < 0) {
            throw MyException(ErrorType::NegativeSize, 0);
        }
        
        this->count = sourceCount;
        if (sourceCount == 0) {
            this->data = nullptr;
            return;
        }
        
        this->data = new T[sourceCount];
        for (int i = 0; i < sourceCount; i++) {
            this->data[i] = source[i];
        }
    }

    void clear() {
        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }
        count = 0;
    }

public:
    ImmutableArraySequence() : data(nullptr), count(0) {}

    ImmutableArraySequence(T* arr, int count) {
        copyFrom(arr, count);
    }

    ImmutableArraySequence(const ImmutableArraySequence<T>& other) {
        copyFrom(other.data, other.count);
    }

    ImmutableArraySequence(const Sequence<T>& other) {
        count = other.GetLength();
        if (count > 0) {
            data = new T[count];
            for (int i = 0; i < count; i++) {
                data[i] = other.Get(i);
            }
        } else {
            data = nullptr;
        }
    }

    virtual ~ImmutableArraySequence() {
        clear();
    }

    ImmutableArraySequence<T>& operator=(const ImmutableArraySequence<T>& other) {
        if (this != &other) {
            clear();
            copyFrom(other.data, other.count);
        }
        return *this;
    }

    virtual T GetFirst() const override {
        if (count == 0) {
            throw MyException(ErrorType::OutOfRange, 3);
        }
        return data[0];
    }

    virtual T GetLast() const override {
        if (count == 0) {
            throw MyException(ErrorType::OutOfRange, 3);
        }
        return data[count - 1];
    }

    virtual T Get(int index) const override {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (index >= count) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        return data[index];
    }

    virtual int GetLength() const override {
        return count;
    }

    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (endIndex < 0 || startIndex > endIndex) {
            throw MyException(ErrorType::OutOfRange, 2);
        }
        if (endIndex >= count) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        
        int newLength = endIndex - startIndex + 1;
        T* subData = new T[newLength];
        
        for (int i = 0; i < newLength; i++) {
            subData[i] = data[startIndex + i];
        }
        
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(subData, newLength);
        delete[] subData;
        return result;
    }

    virtual Sequence<T>* Append(const T& item) override {
        T* newData = new T[count + 1];
        
        for (int i = 0; i < count; i++) {
            newData[i] = data[i];
        }
        
        newData[count] = item;
        
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(newData, count + 1);
        delete[] newData;
        return result;
    }

    virtual Sequence<T>* Prepend(const T& item) override {
        T* newData = new T[count + 1];
        
        newData[0] = item;
        
        for (int i = 0; i < count; i++) {
            newData[i + 1] = data[i];
        }
        
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(newData, count + 1);
        delete[] newData;
        return result;
    }

    virtual Sequence<T>* InsertAt(const T& item, int index) override {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (index > count) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        
        T* newData = new T[count + 1];
        
        for (int i = 0; i < index; i++) {
            newData[i] = data[i];
        }
        
        newData[index] = item;
        
        for (int i = index; i < count; i++) {
            newData[i + 1] = data[i];
        }
        
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(newData, count + 1);
        delete[] newData;
        return result;
    }

    virtual Sequence<T>* Concat(const Sequence<T>* other) const override {
        int otherLength = other->GetLength();
        int newLength = count + otherLength;
        
        T* newData = new T[newLength];
        
        for (int i = 0; i < count; i++) {
            newData[i] = data[i];
        }
        
        for (int i = 0; i < otherLength; i++) {
            newData[count + i] = other->Get(i);
        }
        
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(newData, newLength);
        delete[] newData;
        return result;
    }

    virtual Sequence<T>* RemoveAt(int index) override {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (index >= count) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        if (count == 0) {
            throw MyException(ErrorType::OutOfRange, 3);
        }
        
        T* newData = new T[count - 1];
        
        for (int i = 0; i < index; i++) {
            newData[i] = data[i];
        }
        
        for (int i = index + 1; i < count; i++) {
            newData[i - 1] = data[i];
        }
        
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(newData, count - 1);
        delete[] newData;
        return result;
    }

    virtual const char* TypeName() const override {
        return "ImmutableArraySequence";
    }

    virtual Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }

    bool IsEmpty() const {
        return count == 0;
    }

    bool operator==(const ImmutableArraySequence<T>& other) const {
        if (count != other.count) return false;
        for (int i = 0; i < count; i++) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    bool operator!=(const ImmutableArraySequence<T>& other) const {
        return !(*this == other);
    }
};