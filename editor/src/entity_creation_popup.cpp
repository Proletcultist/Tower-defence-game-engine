#include "editor/entity_creation_popup.h"

#include "imgui.h"

namespace entity_creation_popup {
void OpenPopup() { ImGui::OpenPopup("CreateEntityPopup"); }

EntityType DrawPopup() {
  EntityType selectedType = EntityType::None;

  if (!ImGui::BeginPopup("CreateEntityPopup")) {
    return selectedType;
  }
  if (ImGui::Selectable("GameMapBackground")) {
    selectedType = EntityType::GameMapBackground;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("GameMap")) {
    selectedType = EntityType::GameMap;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("Texture")) {
    selectedType = EntityType::Texture;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("EnemyType")) {
    selectedType = EntityType::EnemyType;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("EnemyWave")) {
    selectedType = EntityType::EnemyWave;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("EnemyWaveOnPath")) {
    selectedType = EntityType::EnemyWaveOnPath;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("EnemyPath")) {
    selectedType = EntityType::EnemyPath;
    ImGui::CloseCurrentPopup();
  }

  if (ImGui::Selectable("TowerType")) {
    selectedType = EntityType::TowerType;
    ImGui::CloseCurrentPopup();
  }

  if (ImGui::Selectable("TowerSpot")) {
    selectedType = EntityType::TowerSpot;
    ImGui::CloseCurrentPopup();
  }

  if (ImGui::Selectable("DamageType")) {
    selectedType = EntityType::DamageType;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Selectable("Ability")) {
    selectedType = EntityType::Ability;
    ImGui::CloseCurrentPopup();
  }

  if (ImGui::Selectable("Effect")) {
    selectedType = EntityType::Effect;
    ImGui::CloseCurrentPopup();
  }

  ImGui::EndPopup();

  return selectedType;
}
} // namespace entity_creation_popup