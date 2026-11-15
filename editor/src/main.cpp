#include "raylib.h"

#include "imgui.h"
#include "rlImGui.h"

#include "menu_bar.h"
#include "hierarchy_panel.h"

int main()
{
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 800;

    SetConfigFlags(
        FLAG_MSAA_4X_HINT |
        FLAG_VSYNC_HINT |
        FLAG_WINDOW_RESIZABLE
    );

    InitWindow(
        screenWidth,
        screenHeight,
        "Tower Defence Level Editor"
    );

    if (!IsWindowReady())
    {
        return 1;
    }

    SetTargetFPS(144);
    rlImGuiSetup(true);

    bool exitRequested = false;

    while (!WindowShouldClose() && !exitRequested)
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();

        exitRequested = DrawMenuBar();
		DrawHierarchyPanel(300.0f, 600.0f);
        rlImGuiEnd();

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}