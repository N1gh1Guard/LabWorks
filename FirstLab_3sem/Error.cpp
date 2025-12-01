#include "Error.h"
#include "Exception.h"
#include <iostream>

std::vector<ErrorInfo> g_ErrorTable = {
    {static_cast<int>(ErrorType::OutOfRange), -1, "Index out of range"},
    {static_cast<int>(ErrorType::InvalidArg), -1, "Invalid argument"},
    {static_cast<int>(ErrorType::NegativeSize), -1, "Negative size specified"},
    
    {static_cast<int>(ErrorType::OptionError), static_cast<int>(OptionErrorCode::UnwrapOnNone), "Unwrap called on None Option"},
    {static_cast<int>(ErrorType::OptionError), static_cast<int>(OptionErrorCode::InvalidMapping), "Invalid type mapping in Option"},
    
    {static_cast<int>(ErrorType::SequenceError), 0, "Sequence type mismatch"},
    {static_cast<int>(ErrorType::SequenceError), 1, "Invalid sequence operation"},
    
    {static_cast<int>(ErrorType::InvalidArg), static_cast<int>(InvalidArgCode::MenuInputNotInteger), "Menu input not an integer?"},
    {static_cast<int>(ErrorType::InvalidArg), static_cast<int>(InvalidArgCode::ArrayInputNotInteger), "Array input was not integer?"},
    {static_cast<int>(ErrorType::InvalidArg), static_cast<int>(InvalidArgCode::NullPointer), "Null pointer passed to function"},

    {static_cast<int>(ErrorType::OutOfRange), 0, "Index < 0?"},
    {static_cast<int>(ErrorType::OutOfRange), 1, "Index >= size?"},
    {static_cast<int>(ErrorType::OutOfRange), 2, "start > end?"},
    {static_cast<int>(ErrorType::OutOfRange), 3, "Empty?"},
    {static_cast<int>(ErrorType::OutOfRange), 4, "Unknown sub-code?"},

    {static_cast<int>(ErrorType::OptionError), static_cast<int>(OptionErrorCode::InvalidMapping), 
     "Invalid mapping in monadic operation"},
    {static_cast<int>(ErrorType::OptionError), static_cast<int>(OptionErrorCode::TupleBindError), 
     "Tuple bind error"}
};

MyException::MyException(ErrorType t, int sc) : type(t), subCode(sc) {
    for (const auto& err : g_ErrorTable) {
        if (static_cast<int>(type) == err.code && 
            (sc == -1 || sc == err.subCode)) {
            message = err.message;
            return;
        }
    }
    message = "Unknown error";
}

std::string getErrorMessage(ErrorType type, int subCode) {
    for (auto &err : g_ErrorTable) {
        if (static_cast<int>(type) == err.code && 
            (subCode == -1 || subCode == err.subCode)) {
            return err.message;
        }
    }
    return "Unknown error";
}

void handleException(const MyException &ex) {
    std::cout << "[Error] " << ex.what() 
              << " (type=" << static_cast<int>(ex.getType())
              << ", subCode=" << ex.getSubCode() << ")\n";
}