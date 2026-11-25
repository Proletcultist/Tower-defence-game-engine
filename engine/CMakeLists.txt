add_executable(tdg-engine)
target_link_libraries(tdg-engine
    PRIVATE
        raylib
)

target_sources(tdg-engine
    PRIVATE
        src/main.cpp
        src/game_state.cpp
        src/engine_state.cpp
)

target_include_directories(tdg-engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)