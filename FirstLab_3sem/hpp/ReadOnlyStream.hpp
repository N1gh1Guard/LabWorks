#pragma once
#include <vector>
#include <fstream>
#include <cstddef>
#include "Exceptions.hpp"

template <typename T>
class ReadOnlyStream {
public:
    explicit ReadOnlyStream(std::vector<T>* seq) 
        : data(seq), isMemory(true), position(0), isOpen(false) {}
    
    explicit ReadOnlyStream(const std::string& filename)
        : filename(filename), isMemory(false), position(0), isOpen(false) {}
    
    void Open() {
        if (isMemory) {
            isOpen = true;
        } else {
            file.open(filename);
            if (!file.is_open()) {
                throw StreamException("Cannot open file: " + filename);
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
    
    T Read() {
        if (!isOpen) {
            throw StreamNotOpenException();
        }
        
        if (IsEndOfStream()) {
            throw EndOfStreamException();
        }
        
        T element;
        
        if (isMemory) {
            element = (*data)[position];
        } else {
            if (!(file >> element)) {
                throw EndOfStreamException();
            }
        }
        
        position++;
        return element;
    }
    
    bool IsEndOfStream() const {
        if (isMemory) {
            return position >= data->size();
        } else {
            return file.eof() || file.fail();
        }
    }
    
    size_t GetPosition() const {
        return position;
    }
    
    bool IsCanSeek() const {
        return isMemory;
    }
    
    bool IsCanGoBack() const {
        return isMemory;
    }
    
    size_t Seek(size_t index) {
        if (!IsCanSeek()) {
            throw SeekNotSupportedException();
        }
        if (index >= data->size()) {
            position = data->size();
        } else {
            position = index;
        }
        return position;
    }
    
    bool GoBack(size_t steps = 1) {
        if (!IsCanGoBack()) {
            throw GoBackNotSupportedException();
        }
        if (position >= steps) {
            position -= steps;
            return true;
        }
        return false;
    }
    
    ~ReadOnlyStream() {
        Close();
    }
    
private:
    std::vector<T>* data = nullptr;
    std::string filename;
    mutable std::ifstream file;
    
    bool isMemory;
    size_t position;
    bool isOpen;
};
