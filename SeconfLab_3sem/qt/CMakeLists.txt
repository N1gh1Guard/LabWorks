cmake_minimum_required(VERSION 3.16)
project(Laba2GUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(SOURCES
    main_qt.cpp
    main_window.cpp
)

set(HEADERS
    main_window.h
)


include_directories(
    ${CMAKE_SOURCE_DIR}/../hpp
    ${CMAKE_SOURCE_DIR}/../test
)


add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)


target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

