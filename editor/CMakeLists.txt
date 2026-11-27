
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
        src/map_view.cpp
        src/inspector_panel.cpp
        src/entity_creation_popup.cpp
        src/hierarchy_view_model.cpp
        src/inspector_combat_properties.cpp
        src/inspector_map_properties.cpp
        src/inspector_reference_widgets.cpp

        include/editor/menu_bar.h
        include/editor/hierarchy_panel.h
        include/editor/map_view.h
        include/editor/inspector_panel.h
        include/editor/entity_creation_popup.h
        include/editor/hierarchy_view_model.h
        include/editor/entity_properties.h
        include/editor/entity.h
        include/editor/inspector_combat_properties.h
        include/editor/inspector_map_properties.h
        include/editor/inspector_reference_widgets.h
)

target_include_directories(tdg-editor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)