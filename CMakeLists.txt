cmake_minimum_required(VERSION 3.16)

project(xmu-calculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(xmu-calculator
    xmudata.cpp
    xmudialog.cpp
    main.cpp
    xmudata.qrc
)

target_link_libraries(xmu-calculator Qt5::Widgets)
