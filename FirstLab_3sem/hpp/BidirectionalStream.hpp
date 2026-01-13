#pragma once
#include "ReadOnlyStream.hpp"
#include "WriteOnlyStream.hpp"

template <typename T>
class BidirectionalStream {
public:
    explicit BidirectionalStream(std::vector<T>* seq)
        : readStream(seq), writeStream(seq) {}
    
    void Open() {
        readStream.Open();
        writeStream.Open();
    }
    
    void Close() {
        readStream.Close();
        writeStream.Close();
    }
    
    T Read() { 
        return readStream.Read(); 
    }
    
    size_t Write(const T& element) { 
        return writeStream.Write(element); 
    }
    
    size_t GetReadPosition() const { 
        return readStream.GetPosition(); 
    }
    
    size_t GetWritePosition() const { 
        return writeStream.GetPosition(); 
    }
    
    bool IsEndOfStream() const { 
        return readStream.IsEndOfStream(); 
    }
    
private:
    ReadOnlyStream<T> readStream;
    WriteOnlyStream<T> writeStream;
};
