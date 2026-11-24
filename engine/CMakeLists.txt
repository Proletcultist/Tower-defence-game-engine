add_executable(tdg-engine)
target_link_libraries(tdg-engine
    PRIVATE
        raylib
)

target_sources(tdg-engine
    PRIVATE
        src/main.cpp
        src/project_folder.cpp

        include/engine/project_folder.h
)

target_include_directories(tdg-engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
