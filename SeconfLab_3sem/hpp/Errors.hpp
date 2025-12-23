#pragma once

#include <stdexcept>
#include <string>

class KeyNotFoundError : public std::out_of_range {
public:
    explicit KeyNotFoundError(const std::string& key_info)
        : std::out_of_range("Key not found: " + key_info) {}
};

class DuplicateKeyError : public std::runtime_error {
public:
    explicit DuplicateKeyError(const std::string& key_info)
        : std::runtime_error("Duplicate key: " + key_info) {}
};
