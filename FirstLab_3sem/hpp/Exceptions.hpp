#pragma once

#include <string>

class StreamException : public std::exception {
public:
    explicit StreamException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
private:
    std::string message;
};

class EndOfStreamException : public StreamException {
public:
    EndOfStreamException() : StreamException("End of stream reached") {}
};

class SeekNotSupportedException : public StreamException {
public:
    SeekNotSupportedException() 
        : StreamException("Seek operation is not supported for this stream") {}
};

class GoBackNotSupportedException : public StreamException {
public:
    GoBackNotSupportedException() 
        : StreamException("GoBack operation is not supported for this stream") {}
};

class StreamClosedException : public StreamException {
public:
    StreamClosedException() 
        : StreamException("Stream is closed") {}
};
