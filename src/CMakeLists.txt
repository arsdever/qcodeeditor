set(CMAKE_AUTOMOC ON)

set(HEADERS qcodeeditor.hpp qcodeeditor_line_numbers_area.hpp)
set(SOURCES qcodeeditor.cpp qcodeeditor_line_numbers_area.cpp)

add_library(qcodeeditor STATIC ${HEADERS} ${SOURCES})
target_include_directories(qcodeeditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(qcodeeditor PRIVATE Qt6::Widgets)
