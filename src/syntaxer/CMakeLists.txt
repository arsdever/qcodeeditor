find_package(nlohmann_json REQUIRED)

add_library(syntaxer_lib STATIC theme.hpp theme.cpp)
add_library(syntaxer::lib ALIAS syntaxer_lib)
target_link_libraries(syntaxer_lib PUBLIC nlohmann_json::nlohmann_json)

add_executable(syntaxer main.cpp)
target_link_libraries(syntaxer PRIVATE syntaxer::lib)
