#include "editor/hierarchy_panel.h"

#include "imgui.h"

namespace hierarchy {
void DrawProjectDefinitions() {
  if (!ImGui::CollapsingHeader("PROJECT DEFINITIONS",
                               ImGuiTreeNodeFlags_DefaultOpen)) {
    return;
  }

  if (ImGui::TreeNodeEx("Enemy Types", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::TreeNodeEx("EnemyType_Boss",
                      ImGuiTreeNodeFlags_Leaf |
                          ImGuiTreeNodeFlags_NoTreePushOnOpen);

    ImGui::TreeNodeEx("EnemyType_Basic",
                      ImGuiTreeNodeFlags_Leaf |
                          ImGuiTreeNodeFlags_NoTreePushOnOpen);

    ImGui::TreePop();
  }

  if (ImGui::TreeNodeEx("Tower Types", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::TreeNodeEx("TowerType_Archer",
                      ImGuiTreeNodeFlags_Leaf |
                          ImGuiTreeNodeFlags_NoTreePushOnOpen);

    ImGui::TreeNodeEx("TowerType_Canon",
                      ImGuiTreeNodeFlags_Leaf |
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
} // namespace hierarchy

void DrawHierarchyPanel(float width, float height) {
  ImGui::BeginChild("HierarchyPanel", ImVec2(width, height), true);

  ImGui::TextUnformatted("HIERARCHY");
  ImGui::Separator();

  hierarchy::DrawProjectDefinitions();

  ImGui::Spacing();

  hierarchy::DrawLevelEntities();

  ImGui::EndChild();
}