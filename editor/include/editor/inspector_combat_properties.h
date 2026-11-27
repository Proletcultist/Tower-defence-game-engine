#pragma once

#include "editor/entity_properties.h"
#include "editor/hierarchy_view_model.h"

namespace inspector::combat_properties {
void DrawEnemyTypeProperties(entity::EnemyTypeProperties &properties,
                             const hierarchy::HierarchyViewModel &viewModel);

void DrawEnemyWaveProperties(entity::EnemyWaveProperties &properties,
                             const hierarchy::HierarchyViewModel &viewModel);

void DrawEnemyWaveOnPathProperties(
    entity::EnemyWaveOnPathProperties &properties,
    const hierarchy::HierarchyViewModel &viewModel);

void DrawTowerTypeProperties(entity::TowerTypeProperties &properties,
                             const hierarchy::HierarchyViewModel &viewModel);

void DrawTowerSpotProperties(entity::TowerSpotProperties &properties,
                             const hierarchy::HierarchyViewModel &viewModel);

void DrawDamageTypeProperties(entity::DamageTypeProperties &properties,
                              const hierarchy::HierarchyViewModel &viewModel);

void DrawAbilityProperties(entity::AbilityProperties &properties,
                           const hierarchy::HierarchyViewModel &viewModel);

void DrawEffectProperties(entity::EffectProperties &properties);
} // namespace inspector::combat_properties