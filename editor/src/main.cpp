#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"

#include "editor/hierarchy_panel.h"
#include "editor/inspector_panel.h"
#include "editor/map_view.h"
#include "editor/menu_bar.h"

int main() {
  constexpr int screenWidth = 1280;
  constexpr int screenHeight = 800;

  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

  InitWindow(screenWidth, screenHeight, "Tower Defence Level Editor");

  if (!IsWindowReady()) {
    return 1;
  }

  SetTargetFPS(144);
  rlImGuiSetup(true);

  bool exitRequested = false;

  menu::Theme currentTheme = menu::Theme::Dark;
  menu::ApplyTheme(currentTheme);

  while (!WindowShouldClose() && !exitRequested) {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    rlImGuiBegin();

    exitRequested = menu::DrawMenuBar(currentTheme);

    ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f));
    ImGui::SetNextWindowSize(
        ImVec2(static_cast<float>(GetScreenWidth()),
               static_cast<float>(GetScreenHeight() - 20)));

    ImGui::Begin("Editor", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    const ImVec2 availableSize = ImGui::GetContentRegionAvail();

    constexpr float hierarchyWidth = 320.0f;
    constexpr float inspectorWidth = 300.0f;
    constexpr float spacing = 8.0f;

    const float mapWidth =
        availableSize.x - hierarchyWidth - inspectorWidth - spacing * 2.0f;

    const float panelHeight = availableSize.y;

    hierarchy::DrawHierarchyPanel(hierarchyWidth, panelHeight);

    ImGui::SameLine(0.0f, spacing);

    map_view::DrawMapView(mapWidth, panelHeight);

    ImGui::SameLine(0.0f, spacing);

    inspector::DrawInspectorPanel(inspectorWidth, panelHeight);

    ImGui::End();

    rlImGuiEnd();

    EndDrawing();
  }

  rlImGuiShutdown();
  CloseWindow();

  return 0;
}