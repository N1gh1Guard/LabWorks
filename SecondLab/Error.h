#pragma once
#include <vector>
#include <string>

enum class ErrorType;

enum OptionErrorCode {
    UnwrapOnNone = 0,
    InvalidMapping = 1,
    TupleBindError = 2
};

struct ErrorInfo {
    int code;          // Основной код ошибки (ErrorType)
    int subCode;       // Дополнительный код ошибки
    std::string message;
};

extern std::vector<ErrorInfo> g_ErrorTable; 

void handleException(const class MyException &ex);
std::string getErrorMessage(ErrorType type, int subCode = -1);