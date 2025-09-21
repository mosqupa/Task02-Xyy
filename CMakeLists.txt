cmake_minimum_required(VERSION 3.10)

project(Task02)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

add_executable(task02
    src/main.cpp
    src/utils.cpp
    src/utils.hpp
    src/armour.hpp
    src/armour.cpp
    src/armor.hpp
    src/armor.cpp
)

target_include_directories(task02 PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(task02 PRIVATE
    ${OpenCV_LIBS}
)