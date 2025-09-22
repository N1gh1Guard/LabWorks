#pragma once
#include <stdexcept>
#include "Exeption.h"

template <class T>
class DynamicArray {
private:
    T* data;
    int size;

public:
    DynamicArray(const T* items, int count) {
        if (count < 0) {
            throw MyException(ErrorType::NegativeSize, 0);
        }
        size = count;
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = items[i];
        }
    }

    DynamicArray(int size) {
        if (size < 0) {
            throw MyException(ErrorType::NegativeSize, 0);
        }
        this->size = size;
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = T();
        }
    }

    DynamicArray(const DynamicArray<T>& other) {
        size = other.size;
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    ~DynamicArray() {
        delete[] data;
    }

    int GetSize() const {
        return size;
    }


    T Get(int index) const {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 0); // code=0 => "index < 0?"
        }
        if (index >= size) {
            throw MyException(ErrorType::OutOfRange, 1); // code=1 => "index >= size"
        }
        return data[index];
    }

    void Set(int index, const T& value) {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (index >= size) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        T* newData = new T[newSize];
        int minSize = (newSize < size) ? newSize : size;
        for (int i = 0; i < minSize; i++) {
            newData[i] = data[i];
        }
        for (int i = minSize; i < newSize; i++) {
            newData[i] = T();
        }
        delete[] data;
        data = newData;
        size = newSize;
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new T[size];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    T& operator[](int index) {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (index >= size) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        return data[index];
    }
    const T& operator[](int index) const {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (index >= size) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        return data[index];
    }
};