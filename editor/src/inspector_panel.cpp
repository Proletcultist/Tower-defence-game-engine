#include "editor/inspector_panel.h"

#include "editor/inspector_combat_properties.h"
#include "editor/inspector_map_properties.h"

#include "imgui.h"

#include <cstdio>
#include <type_traits>
#include <variant>

namespace inspector {
namespace {
const char *GetEntityTypeName(entity::EntityType type) {
  switch (type) {
  case entity::EntityType::None:
    return "None";
  case entity::EntityType::Level:
    return "Level";
  case entity::EntityType::GameMapBackground:
    return "GameMapBackground";
  case entity::EntityType::GameMap:
    return "GameMap";
  case entity::EntityType::Texture:
    return "Texture";
  case entity::EntityType::EnemyType:
    return "EnemyType";
  case entity::EntityType::EnemyWave:
    return "EnemyWave";
  case entity::EntityType::EnemyWaveOnPath:
    return "EnemyWaveOnPath";
  case entity::EntityType::EnemyPath:
    return "EnemyPath";
  case entity::EntityType::TowerType:
    return "TowerType";
  case entity::EntityType::TowerSpot:
    return "TowerSpot";
  case entity::EntityType::DamageType:
    return "DamageType";
  case entity::EntityType::Ability:
    return "Ability";
  case entity::EntityType::Effect:
    return "Effect";
  }

  return "Unknown";
}

void DrawProperties(entity::EntityProperties &properties,
                    const hierarchy::HierarchyViewModel &viewModel) {
  std::visit(
      [&viewModel](auto &value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, std::monostate> ||
                      std::is_same_v<T, entity::LevelProperties>) {
          ImGui::TextUnformatted("No editable properties.");
        } else if constexpr (std::is_same_v<
                                 T, entity::GameMapBackgroundProperties>) {
          map_properties::DrawGameMapBackgroundProperties(value, viewModel);
        } else if constexpr (std::is_same_v<T, entity::GameMapProperties>) {
          map_properties::DrawGameMapProperties(value, viewModel);
        } else if constexpr (std::is_same_v<T, entity::TextureProperties>) {
          map_properties::DrawTextureProperties(value);
        } else if constexpr (std::is_same_v<T, entity::EnemyPathProperties>) {
          map_properties::DrawEnemyPathProperties(value);
        } else if constexpr (std::is_same_v<T, entity::EnemyTypeProperties>) {
          combat_properties::DrawEnemyTypeProperties(value, viewModel);
        } else if constexpr (std::is_same_v<T, entity::EnemyWaveProperties>) {
          combat_properties::DrawEnemyWaveProperties(value, viewModel);
        } else if constexpr (std::is_same_v<
                                 T, entity::EnemyWaveOnPathProperties>) {
          combat_properties::DrawEnemyWaveOnPathProperties(value, viewModel);
        } else if constexpr (std::is_same_v<T, entity::TowerTypeProperties>) {
          combat_properties::DrawTowerTypeProperties(value, viewModel);
        } else if constexpr (std::is_same_v<T, entity::TowerSpotProperties>) {
          combat_properties::DrawTowerSpotProperties(value, viewModel);
        } else if constexpr (std::is_same_v<T, entity::DamageTypeProperties>) {
          combat_properties::DrawDamageTypeProperties(value, viewModel);
        } else if constexpr (std::is_same_v<T, entity::AbilityProperties>) {
          combat_properties::DrawAbilityProperties(value, viewModel);
        } else if constexpr (std::is_same_v<T, entity::EffectProperties>) {
          combat_properties::DrawEffectProperties(value);
        }
      },
      properties);
}
} // namespace

void DrawInspectorPanel(float width, float height,
                        hierarchy::HierarchyViewModel &viewModel) {
  ImGui::BeginChild("InspectorPanel", ImVec2(width, height), true);

  ImGui::TextUnformatted("PROPERTIES");
  ImGui::Separator();

  const auto &selected = viewModel.GetSelected();

  if (!selected) {
    ImGui::TextWrapped("Select an entity from the hierarchy.");
    ImGui::EndChild();
    return;
  }

  hierarchy::HierarchyItem *item = viewModel.FindEntity(selected->entityId);

  if (item == nullptr) {
    ImGui::TextWrapped("The selected entity could not be found.");
    ImGui::EndChild();
    return;
  }

  char nameBuffer[256]{};
  std::snprintf(nameBuffer, sizeof(nameBuffer), "%s", item->name.c_str());

  if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer))) {
    item->name = nameBuffer;
  }

  ImGui::Text("Type: %s", GetEntityTypeName(item->type));
  ImGui::Separator();

  DrawProperties(item->properties, viewModel);

  ImGui::EndChild();
}
} // namespace inspector