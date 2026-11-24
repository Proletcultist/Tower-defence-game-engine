#include "editor/hierarchy_panel.h"

#include "editor/entity_creation_popup.h"

#include "imgui.h"

namespace hierarchy {
void DrawProjectDefinitions() {
  if (!ImGui::CollapsingHeader("PROJECT DEFINITIONS",
                               ImGuiTreeNodeFlags_DefaultOpen)) {
    return;
  }

  if (ImGui::TreeNodeEx("Enemies", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::TreeNodeEx("Enemy_Boss", ImGuiTreeNodeFlags_Leaf |
                                        ImGuiTreeNodeFlags_NoTreePushOnOpen);

    ImGui::TreeNodeEx("Enemy_Basic", ImGuiTreeNodeFlags_Leaf |
                                         ImGuiTreeNodeFlags_NoTreePushOnOpen);

    ImGui::TreePop();
  }

  if (ImGui::TreeNodeEx("Towers", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::TreeNodeEx("Tower_Archer", ImGuiTreeNodeFlags_Leaf |
                                          ImGuiTreeNodeFlags_NoTreePushOnOpen);

    ImGui::TreeNodeEx("Tower_Canon", ImGuiTreeNodeFlags_Leaf |
                                         ImGuiTreeNodeFlags_NoTreePushOnOpen);

    ImGui::TreePop();
  }

  if (ImGui::TreeNodeEx("Abilities")) {
    ImGui::TreeNodeEx("Ability_Fireball",
                      ImGuiTreeNodeFlags_Leaf |
                          ImGuiTreeNodeFlags_NoTreePushOnOpen);

    ImGui::TreePop();
  }

  if (ImGui::TreeNodeEx("Effects")) {
    ImGui::TreeNodeEx("Effect_Poison", ImGuiTreeNodeFlags_Leaf |
                                           ImGuiTreeNodeFlags_NoTreePushOnOpen);

    ImGui::TreePop();
  }
}

void DrawLevelEntities() {
  if (!ImGui::CollapsingHeader("LEVEL ENTITIES",
                               ImGuiTreeNodeFlags_DefaultOpen)) {
    return;
  }

  if (ImGui::TreeNodeEx("GameLevel_1", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::TreeNodeEx("GameMap", ImGuiTreeNodeFlags_Leaf |
                                     ImGuiTreeNodeFlags_NoTreePushOnOpen);

    ImGui::TreeNodeEx("EnemyPath_1", ImGuiTreeNodeFlags_Leaf |
                                         ImGuiTreeNodeFlags_NoTreePushOnOpen);

    ImGui::TreeNodeEx("TowerSpot_1", ImGuiTreeNodeFlags_Leaf |
                                         ImGuiTreeNodeFlags_NoTreePushOnOpen);

    if (ImGui::TreeNodeEx("EnemyWave_1")) {
      ImGui::TreeNodeEx("EnemyType_Basic",
                        ImGuiTreeNodeFlags_Leaf |
                            ImGuiTreeNodeFlags_NoTreePushOnOpen);

      ImGui::TreeNodeEx("EnemyType_Boss",
                        ImGuiTreeNodeFlags_Leaf |
                            ImGuiTreeNodeFlags_NoTreePushOnOpen);

      ImGui::TreePop();
    }

    ImGui::TreePop();
  }
}

void DrawHierarchyPanel(float width, float height) {
  ImGui::BeginChild("HierarchyPanel", ImVec2(width, height), true);

  ImGui::TextUnformatted("HIERARCHY");

  constexpr float addButtonWidth = 22.0f;
  constexpr float rightPadding = 6.0f;

  ImGui::SameLine(ImGui::GetWindowWidth() - addButtonWidth - rightPadding);

  if (ImGui::Button("+", ImVec2(addButtonWidth, 0.0f))) {
    entity_creation_popup::OpenPopup();
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Create entity");
  }

  ImGui::Separator();

  DrawProjectDefinitions();
  ImGui::Spacing();
  DrawLevelEntities();

  const entity_creation_popup::EntityType selectedType =
      entity_creation_popup::DrawPopup();

  if (selectedType != entity_creation_popup::EntityType::None) {
  }

  ImGui::EndChild();
}
} // namespace hierarchy