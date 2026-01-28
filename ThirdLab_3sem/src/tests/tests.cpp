#include "tests.hpp"
#include <stdexcept>

int main() {
    try {
        AlgorithmTests::RunAllTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении тестов: " << e.what() << std::endl;
        return 1;
    }
}
