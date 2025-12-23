#pragma once

#include <ostream>
#include <string>
#include <tuple>
#include <vector>

struct User {
    std::string name;
    int         age{};
    std::string email;
    int         id{};
};

inline bool operator==(const User& lhs, const User& rhs) {
    return std::tie(lhs.name, lhs.age, lhs.email, lhs.id) ==
           std::tie(rhs.name, rhs.age, rhs.email, rhs.id);
}

inline bool operator!=(const User& lhs, const User& rhs) {
    return !(lhs == rhs);
}

inline std::ostream& operator<<(std::ostream& os, const User& user) {
    os << "User{name=" << user.name
       << ", age=" << user.age
       << ", email=" << user.email
       << ", id=" << user.id << "}";
    return os;
}

struct Page {
    std::size_t word_limit{0};     // максимальное количество слов или символов
    std::size_t used{0};           // фактически занято слов/символов
    std::vector<std::string> words;
};
