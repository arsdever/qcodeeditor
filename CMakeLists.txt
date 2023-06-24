cmake_minimum_required(VERSION 3.10)

project(qcodeeditor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(spdlog REQUIRED)
link_libraries(spdlog::spdlog)

add_subdirectory(src)
add_subdirectory(sample)
