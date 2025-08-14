#pragma once
#include <stdexcept>
#include <string>
#include "Error.h"  // Добавлено включение Error.h

enum class ErrorType {
    OutOfRange,
    InvalidArg,
    NegativeSize,
    OptionError,
    SequenceError
};

class MyException : public std::exception {
private:
    ErrorType type;
    int subCode;
    std::string message;
    
public:
    MyException(ErrorType t, int sc);
    
    ErrorType getType() const { return type; }
    int getSubCode() const { return subCode; }
    const char* what() const noexcept override {
        return message.c_str();
    }
};