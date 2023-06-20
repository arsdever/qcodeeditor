set(HEADERS qcodeeditor.hpp)
set(SOURCES qcodeeditor.cpp)

add_library(qcodeeditor STATIC ${HEADERS} ${SOURCES})
target_include_directories(qcodeeditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(qcodeeditor PRIVATE Qt6::Widgets)
