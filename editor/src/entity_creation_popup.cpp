#include "editor/entity_creation_popup.h"

#include "imgui.h"

namespace entity_creation_popup {
void OpenPopup() { ImGui::OpenPopup("CreateEntityPopup"); }

entity::EntityType DrawPopup() {
  entity::EntityType selectedType = entity::EntityType::None;

  if (!ImGui::BeginPopup("CreateEntityPopup")) {
    return selectedType;
  }
  if (ImGui::Selectable("GameMapBackground")) {
    selectedType = entity::EntityType::GameMapBackground;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("GameMap")) {
    selectedType = entity::EntityType::GameMap;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("Texture")) {
    selectedType = entity::EntityType::Texture;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("EnemyType")) {
    selectedType = entity::EntityType::EnemyType;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("EnemyWave")) {
    selectedType = entity::EntityType::EnemyWave;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("EnemyWaveOnPath")) {
    selectedType = entity::EntityType::EnemyWaveOnPath;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("EnemyPath")) {
    selectedType = entity::EntityType::EnemyPath;
    ImGui::CloseCurrentPopup();
  }

  if (ImGui::Selectable("TowerType")) {
    selectedType = entity::EntityType::TowerType;
    ImGui::CloseCurrentPopup();
  }

  if (ImGui::Selectable("TowerSpot")) {
    selectedType = entity::EntityType::TowerSpot;
    ImGui::CloseCurrentPopup();
  }

  if (ImGui::Selectable("DamageType")) {
    selectedType = entity::EntityType::DamageType;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("Ability")) {
    selectedType = entity::EntityType::Ability;
    ImGui::CloseCurrentPopup();
  }

  if (ImGui::Selectable("Effect")) {
    selectedType = entity::EntityType::Effect;
    ImGui::CloseCurrentPopup();
  }

  ImGui::EndPopup();

  return selectedType;
}
} // namespace entity_creation_popup