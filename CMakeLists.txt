cmake_minimum_required(VERSION 3.16)

project(xmu-calculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

qt_add_executable(xmu-calculator
    xmudata.cpp
    xmudialog.cpp
    main.cpp
    xmudata.qrc
)

target_link_libraries(xmu-calculator PRIVATE Qt6::Core Qt6::Widgets)
