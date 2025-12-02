#include "Exception.h"

MyException::MyException(int code, const std::string& msg) : errorCode(code), message(msg) {}

int MyException::getErrorCode() const {
    return errorCode;
}

const char* MyException::what() const {
    return message.c_str();
}