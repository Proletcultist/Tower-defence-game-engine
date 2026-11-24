#include "editor/hierarchy_panel.h"
#include "editor/entity_creation_popup.h"

#include "imgui.h"

namespace hierarchy {
void DrawProjectDefinitions(HierarchyViewModel &viewModel) {
  if (!ImGui::CollapsingHeader("PROJECT DEFINITIONS",
                               ImGuiTreeNodeFlags_DefaultOpen)) {
    return;
  }

  const auto drawCategory =
      [&viewModel](const char *categoryName,
                   entity_creation_popup::EntityType type) {
        const std::vector<HierarchyItem> *items = viewModel.FindItems(type);

        if (items == nullptr || items->empty()) {
          return;
        }

        if (ImGui::TreeNodeEx(categoryName)) {
          for (const HierarchyItem &item : *items) {
            ImGui::TreeNodeEx(item.name.c_str(),
                              ImGuiTreeNodeFlags_Leaf |
                                  ImGuiTreeNodeFlags_NoTreePushOnOpen);
          }

          ImGui::TreePop();
        }
      };

  drawCategory("Game Map Backgrounds",
               entity_creation_popup::EntityType::GameMapBackground);

  drawCategory("Game Maps", entity_creation_popup::EntityType::GameMap);

  drawCategory("Textures", entity_creation_popup::EntityType::Texture);

  drawCategory("Enemies", entity_creation_popup::EntityType::EnemyType);

  drawCategory("Towers", entity_creation_popup::EntityType::TowerType);

  drawCategory("Damage Types", entity_creation_popup::EntityType::DamageType);

  drawCategory("Abilities", entity_creation_popup::EntityType::Ability);

  drawCategory("Effects", entity_creation_popup::EntityType::Effect);
}

void DrawLevelEntities(HierarchyViewModel &viewModel) {
  if (!ImGui::CollapsingHeader("LEVEL ENTITIES",
                               ImGuiTreeNodeFlags_DefaultOpen)) {
    return;
  }

  const auto drawCategory =
      [&viewModel](const char *categoryName,
                   entity_creation_popup::EntityType type) {
        const std::vector<HierarchyItem> *items = viewModel.FindItems(type);

        if (items == nullptr || items->empty()) {
          return;
        }

        if (ImGui::TreeNodeEx(categoryName)) {
          for (const HierarchyItem &item : *items) {
            ImGui::TreeNodeEx(item.name.c_str(),
                              ImGuiTreeNodeFlags_Leaf |
                                  ImGuiTreeNodeFlags_NoTreePushOnOpen);
          }

          ImGui::TreePop();
        }
      };

  drawCategory("Enemy Waves", entity_creation_popup::EntityType::EnemyWave);

  drawCategory("Enemy Waves On Paths",
               entity_creation_popup::EntityType::EnemyWaveOnPath);

  drawCategory("Enemy Paths", entity_creation_popup::EntityType::EnemyPath);

  drawCategory("Tower Spots", entity_creation_popup::EntityType::TowerSpot);
}

void DrawHierarchyPanel(float width, float height,
                        HierarchyViewModel &viewModel) {
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

  const entity_creation_popup::EntityType selectedType =
      entity_creation_popup::DrawPopup();

  if (selectedType != entity_creation_popup::EntityType::None) {
    viewModel.Add(selectedType);
  }

  DrawProjectDefinitions(viewModel);
  ImGui::Spacing();
  DrawLevelEntities(viewModel);

  ImGui::EndChild();
}
} // namespace hierarchy