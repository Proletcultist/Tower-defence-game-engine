#include "editor/hierarchy_panel.h"

#include "editor/entity_creation_popup.h"

#include "imgui.h"

#include <optional>
#include <string>

namespace hierarchy {
namespace {
void DrawCategory(HierarchyViewModel &viewModel, const char *categoryName,
                  entity::EntityType type) {
  const std::vector<HierarchyItem> *items = viewModel.FindItems(type);

  if (items == nullptr || items->empty()) {
    return;
  }

  if (!ImGui::TreeNodeEx(categoryName)) {
    return;
  }

  std::optional<std::string> nameToDelete;

  for (const HierarchyItem &item : *items) {
    const auto &selected = viewModel.GetSelected();

    const bool isSelected =
        selected && selected->type == type && selected->name == item.name;

    ImGui::PushID(item.name.c_str());

    if (ImGui::Selectable(item.name.c_str(), isSelected)) {
      viewModel.Select(type, item.name);
    }

    if (ImGui::BeginPopupContextItem("EntityContextMenu")) {
      if (ImGui::MenuItem("Delete")) {
        nameToDelete = item.name;
      }

      ImGui::EndPopup();
    }

    ImGui::PopID();
  }

  ImGui::TreePop();

  if (nameToDelete) {
    viewModel.Remove(type, *nameToDelete);
  }
}

void DrawProjectDefinitions(HierarchyViewModel &viewModel) {
  if (!ImGui::CollapsingHeader("PROJECT DEFINITIONS",
                               ImGuiTreeNodeFlags_DefaultOpen)) {
    return;
  }

  DrawCategory(viewModel, "Game Map Backgrounds",
               entity::EntityType::GameMapBackground);

  DrawCategory(viewModel, "Game Maps",
               entity::EntityType::GameMap);

  DrawCategory(viewModel, "Textures",
               entity::EntityType::Texture);

  DrawCategory(viewModel, "Enemies",
               entity::EntityType::EnemyType);

  DrawCategory(viewModel, "Towers",
               entity::EntityType::TowerType);

  DrawCategory(viewModel, "Damage Types",
               entity::EntityType::DamageType);

  DrawCategory(viewModel, "Abilities",
               entity::EntityType::Ability);

  DrawCategory(viewModel, "Effects", entity::EntityType::Effect);
}

void DrawLevelEntities(HierarchyViewModel &viewModel) {
  if (!ImGui::CollapsingHeader("LEVEL ENTITIES",
                               ImGuiTreeNodeFlags_DefaultOpen)) {
    return;
  }

  DrawCategory(viewModel, "Enemy Waves",
               entity::EntityType::EnemyWave);

  DrawCategory(viewModel, "Enemy Waves On Paths",
               entity::EntityType::EnemyWaveOnPath);

  DrawCategory(viewModel, "Enemy Paths",
               entity::EntityType::EnemyPath);

  DrawCategory(viewModel, "Tower Spots",
               entity::EntityType::TowerSpot);
}
} // namespace

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

  const entity::EntityType newType =
      entity_creation_popup::DrawPopup();

  if (newType != entity::EntityType::None) {
    viewModel.Add(newType);
  }

  DrawProjectDefinitions(viewModel);
  ImGui::Spacing();
  DrawLevelEntities(viewModel);

  if (!ImGui::GetIO().WantTextInput && ImGui::IsKeyPressed(ImGuiKey_Delete)) {
    const auto selected = viewModel.GetSelected();

    if (selected) {
      viewModel.Remove(selected->type, selected->name);
    }
  }

  ImGui::EndChild();
}
} // namespace hierarchy