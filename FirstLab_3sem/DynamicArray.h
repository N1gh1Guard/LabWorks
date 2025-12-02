#pragma once

#include "Exception.h"

template <class T>
class DynamicArray {
private:
    T* data;
    int size;

public:
    DynamicArray(const T* items, int count) {
        if (count < 0) {
            throw MyException(1, "Negative size specified");
        }
        size = count;
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = items[i];
        }
    }

    DynamicArray(int size) {
        if (size < 0) {
            throw MyException(1, "Negative size specified");
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
            throw MyException(2, "Index out of range");
        }
        if (index >= size) {
            throw MyException(2, "Index out of range");
        }
        return data[index];
    }

    void Set(int index, const T& value) {
        if (index < 0) {
            throw MyException(2, "Index out of range");
        }
        if (index >= size) {
            throw MyException(2, "Index out of range");
        }
        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw MyException(1, "Negative size specified");
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
            throw MyException(2, "Index out of range");
        }
        if (index >= size) {
            throw MyException(2, "Index out of range");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0) {
            throw MyException(2, "Index out of range");
        }
        if (index >= size) {
            throw MyException(2, "Index out of range");
        }
        return data[index];
    }
};