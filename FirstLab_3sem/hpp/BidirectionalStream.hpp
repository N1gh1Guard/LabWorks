#pragma once
#include <cstddef>
#include <string>
#include "Exceptions.hpp"

template <typename T>
class StreamBase {
public:
    virtual ~StreamBase() = default;
    
    // Управление потоком
    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual bool IsOpen() const = 0;
    
    // Информация о позиции
    virtual size_t GetPosition() const = 0;
    
    // Поддержка операций
    virtual bool IsCanSeek() const { return false; }
    virtual bool IsCanGoBack() const { return false; }
    virtual bool IsEndOfStream() const = 0;
    
protected:
    size_t position = 0;
    bool isOpen = false;
};
