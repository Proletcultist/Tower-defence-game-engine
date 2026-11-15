
add_executable(tdg-editor)
target_link_libraries(tdg-editor
    PRIVATE
        raylib
        imgui
        rlimgui
)
target_sources(tdg-editor
    PRIVATE
        src/main.cpp
        src/menu_bar.cpp
        src/hierarchy_panel.cpp
)
