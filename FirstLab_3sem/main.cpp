#include <QApplication>
#include "mainwindow.hpp"
#include <iostream>
#include <string>
#include "SpaceRemover.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    std::cout << "Введите строку:" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::string result = SpaceRemover::RemoveMultipleSpacesWithDebugOutput(input);

    std::cout << "Результат: " << result << std::endl;

    return 0;
}
