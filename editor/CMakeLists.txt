cmake_minimum_required(VERSION 3.15)
project(tower-defence-game-editor CXX)

set(CMAKE_CXX_STANDARD 20)
# Fail during configuration if compiler doesn't support required c++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add warning flags
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_executable(tdg-editor)
target_sources(tdg-editor
    PRIVATE
        src/main.cpp
)

install(TARGETS tdg-editor DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
