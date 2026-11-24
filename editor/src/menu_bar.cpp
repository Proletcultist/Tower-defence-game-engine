#include "editor/menu_bar.h"

#include "imgui.h"
#include <cmath>

namespace menu {
void ApplyTheme(Theme theme) {
  if (theme == Theme::Light) {
    ImGui::StyleColorsLight();
  } else {
    ImGui::StyleColorsDark();
  }
}

bool DrawMenuBar(Theme &currentTheme) {
  bool exitRequested = false;

  if (!ImGui::BeginMainMenuBar()) {
    return false;
  }

  if (ImGui::BeginMenu("File")) {
    ImGui::MenuItem("New");
    ImGui::MenuItem("Open", "Ctrl+O");
    ImGui::MenuItem("Save", "Ctrl+S");

    ImGui::Separator();

    if (ImGui::MenuItem("Exit")) {
      exitRequested = true;
    }

    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Edit")) {
    ImGui::MenuItem("Undo", "Ctrl+Z", false, false);
    ImGui::MenuItem("Redo", "Ctrl+Y", false, false);

    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("View")) {
    ImGui::MenuItem("Project");
    ImGui::MenuItem("Inspector");
    ImGui::MenuItem("Grid");

    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Help")) {
    ImGui::MenuItem("About");

    ImGui::EndMenu();
  }

constexpr float themeButtonWidth = 36.0f;
constexpr float themeButtonPadding = 8.0f;

ImGui::SameLine(
    ImGui::GetWindowWidth() -
    themeButtonWidth -
    themeButtonPadding
);

ImGui::InvisibleButton(
    "ThemeToggle",
    ImVec2(themeButtonWidth, 20.0f)
);

const bool themeChanged =
    ImGui::IsItemClicked();

ImDrawList* drawList =
    ImGui::GetWindowDrawList();

const ImVec2 buttonMin =
    ImGui::GetItemRectMin();

const ImVec2 iconCenter(
    buttonMin.x + themeButtonWidth * 0.5f,
    buttonMin.y + 10.0f
);

const ImU32 iconColor =
    ImGui::GetColorU32(ImGuiCol_Text);

if (currentTheme == Theme::Light)
{
    drawList->AddCircleFilled(
        iconCenter,
        7.0f,
        iconColor,
        32
    );

    drawList->AddCircleFilled(
        ImVec2(
            iconCenter.x + 4.0f,
            iconCenter.y - 2.0f
        ),
        7.0f,
        ImGui::GetColorU32(ImGuiCol_MenuBarBg)
    );
}
else
{
    drawList->AddCircleFilled(
        iconCenter,
        4.0f,
        iconColor,
        32
    );

    for (int i = 0; i < 8; ++i)
    {
        const float angle =
            static_cast<float>(i) *
            3.14159265f / 4.0f;

        drawList->AddLine(
            ImVec2(
                iconCenter.x +
                    cosf(angle) * 7.0f,
                iconCenter.y +
                    sinf(angle) * 7.0f
            ),
            ImVec2(
                iconCenter.x +
                    cosf(angle) * 9.0f,
                iconCenter.y +
                    sinf(angle) * 9.0f
            ),
            iconColor,
            1.2f
        );
    }
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip(
        currentTheme == Theme::Light
            ? "Switch to dark theme"
            : "Switch to light theme"
    );
}

if (themeChanged)
{
    currentTheme =
        currentTheme == Theme::Light
            ? Theme::Dark
            : Theme::Light;

    ApplyTheme(currentTheme);
}

  ImGui::EndMainMenuBar();

  return exitRequested;
}
} // namespace menu