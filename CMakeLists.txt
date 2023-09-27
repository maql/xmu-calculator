cmake_minimum_required(VERSION 3.16)

project(xmu-calculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

add_executable(xmu-calculator
    xmudata.cpp
    xmudialog.cpp
    main.cpp
    xmudata.qrc
)

target_link_libraries(xmu-calculator Qt5::Widgets Qt5::Core)
