#pragma once
#include <vector>
#include <fstream>
#include <cstddef>
#include "Exceptions.hpp"

template <typename T>
class WriteOnlyStream {
public:
    explicit WriteOnlyStream(std::vector<T>* seq)
        : data(seq), isMemory(true), position(0), isOpen(false) {}
    
    explicit WriteOnlyStream(const std::string& filename)
        : filename(filename), isMemory(false), position(0), isOpen(false) {}
    
    void Open() {
        if (isMemory) {
            isOpen = true;
        } else {
            file.open(filename, std::ios::app);
            if (!file.is_open()) {
                throw StreamException("Cannot open file for writing: " + filename);
            }
            isOpen = true;
        }
    }
    
    void Close() {
        if (!isMemory && file.is_open()) {
            file.close();
        }
        isOpen = false;
    }
    
    size_t Write(const T& element) {
        if (!isOpen) {
            throw StreamNotOpenException();
        }
        
        if (isMemory) {
            data->push_back(element);
        } else {
            file << element << "\n";
            file.flush();
        }
        
        return ++position;
    }
    
    size_t GetPosition() const {
        return position;
    }
    
    ~WriteOnlyStream() {
        Close();
    }
    
private:
    std::vector<T>* data = nullptr;
    std::string filename;
    std::ofstream file;
    
    bool isMemory;
    size_t position;
    bool isOpen;
};
