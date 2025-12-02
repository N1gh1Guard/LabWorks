#pragma once
#include <string>

class MyException {
private:
    int errorCode;
    std::string message;
    
public:
    MyException(int code, const std::string& msg);
    
    int getErrorCode() const;
    const char* what() const;
};