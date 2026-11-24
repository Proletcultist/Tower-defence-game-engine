#pragma once

namespace entity_creation_popup {
enum class EntityType {
  None,
  GameMapBackground,
  GameMap,
  Texture,
  EnemyType,
  EnemyWave,
  EnemyWaveOnPath,
  EnemyPath,
  TowerType,
  TowerSpot,
  DamageType,
  Ability,
  Effect
};

void OpenPopup();

EntityType DrawPopup();
} // namespace entity_creation_popup