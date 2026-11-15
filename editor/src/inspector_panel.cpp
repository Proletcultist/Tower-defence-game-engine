#include "inspector_panel.h"

#include "imgui.h"

void DrawInspectorPanel(float width, float height)
{
    ImGui::BeginChild(
        "InspectorPanel",
        ImVec2(width, height),
        true
    );

    ImGui::TextUnformatted("PROPERTIES");
    ImGui::Separator();

    ImGui::TextUnformatted("Selected entity:");
    ImGui::TextUnformatted("None");

    ImGui::Spacing();
    ImGui::TextUnformatted("Select an entity from the hierarchy.");

    ImGui::EndChild();
}