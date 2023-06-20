add_executable(sample_app main.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

target_link_libraries(sample_app PRIVATE Qt::Core Qt::Gui Qt::Widgets qcodeeditor)
