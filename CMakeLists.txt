cmake_minimum_required(VERSION 3.15)
project(tower-defence-game-engine CXX)

set(CMAKE_CXX_STANDARD 20)
# Fail during configuration if compiler doesn't support required c++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add warning flags
add_compile_options(-Wall -Wextra -Wpedantic)

set(CPM_SOURCE_CACHE ${CMAKE_CURRENT_BINARY_DIR}/.cpm-cache)

# Add CPM for dependencies management
include(cmake/CPM.cmake)

CPMAddPackage("https://github.com/raysan5/raylib.git#6.0")
CPMAddPackage(
    URI "https://github.com/ocornut/imgui.git#v1.92.7"
    PATCHES "patches/imgui-Add-CMakeLists.txt.patch"
)
CPMAddPackage(
    URI "https://github.com/raylib-extras/rlImGui.git#Raylib_6_0"
    PATCHES "patches/rlimgui-Add-CMakeLists.txt.patch"
)

add_subdirectory(editor)
