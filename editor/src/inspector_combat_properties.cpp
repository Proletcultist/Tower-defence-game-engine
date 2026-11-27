#include "editor/inspector_combat_properties.h"

#include "editor/inspector_reference_widgets.h"

#include "imgui.h"

#include <type_traits>
#include <variant>

namespace inspector::combat_properties {
void DrawEnemyTypeProperties(entity::EnemyTypeProperties &properties,
                             const hierarchy::HierarchyViewModel &viewModel) {
  widgets::DrawEntityReference("Texture", properties.texture,
                               entity::EntityType::Texture, viewModel);
  ImGui::InputDouble("Speed", &properties.speed);
  widgets::DrawUnsigned("Hit points", properties.hitPoints);
  widgets::DrawUnsigned("Reward", properties.reward);
  widgets::DrawEffectMultipliers(properties.effectMultipliers, viewModel);
}

void DrawEnemyWaveProperties(entity::EnemyWaveProperties &properties,
                             const hierarchy::HierarchyViewModel &viewModel) {
  widgets::DrawEntityReferenceList("Paths", properties.paths,
                                   entity::EntityType::EnemyWaveOnPath,
                                   viewModel);
  widgets::DrawUnsigned("Duration", properties.duration);
  widgets::DrawEntityReference("Next wave", properties.nextWave,
                               entity::EntityType::EnemyWave, viewModel);
}

void DrawEnemyWaveOnPathProperties(
    entity::EnemyWaveOnPathProperties &properties,
    const hierarchy::HierarchyViewModel &viewModel) {
  widgets::DrawEntityReference("Path", properties.path,
                               entity::EntityType::EnemyPath, viewModel);
  widgets::DrawEntityReferenceList("Enemy types", properties.enemyTypes,
                                   entity::EntityType::EnemyType, viewModel);
  widgets::DrawUnsigned("Spawn rate", properties.spawnRate);
}

void DrawTowerTypeProperties(entity::TowerTypeProperties &properties,
                             const hierarchy::HierarchyViewModel &viewModel) {
  widgets::DrawEntityReference("Texture", properties.texture,
                               entity::EntityType::Texture, viewModel);
  widgets::DrawEntityReference("Damage type", properties.damageType,
                               entity::EntityType::DamageType, viewModel);
  ImGui::InputDouble("Attack range", &properties.attackRange);
  ImGui::InputInt("Cooldown", &properties.cooldown);
  widgets::DrawUnsigned("Sell price", properties.sellPrice);
  widgets::DrawEntityReferencePairs("Promotions", properties.promotions,
                                    "Required level",
                                    entity::EntityType::TowerType, viewModel);
}

void DrawTowerSpotProperties(entity::TowerSpotProperties &properties,
                             const hierarchy::HierarchyViewModel &viewModel) {
  ImGui::InputDouble("X", &properties.x);
  ImGui::InputDouble("Y", &properties.y);
  widgets::DrawEntityReference("Texture", properties.texture,
                               entity::EntityType::Texture, viewModel);
  widgets::DrawEntityReferencePairs("Buildable towers",
                                    properties.buildableTowers, "Price",
                                    entity::EntityType::TowerType, viewModel);
}

void DrawDamageTypeProperties(entity::DamageTypeProperties &properties,
                              const hierarchy::HierarchyViewModel &viewModel) {
  widgets::DrawEntityReference("Texture", properties.texture,
                               entity::EntityType::Texture, viewModel);
  widgets::DrawEntityReferenceList("Effects", properties.effects,
                                   entity::EntityType::Effect, viewModel);

  const char *kinds[] = {"Projectile", "InstantReach", "HomingProjectile"};

  int kind = static_cast<int>(properties.kind);

  if (ImGui::Combo("Damage kind", &kind, kinds, 3)) {
    properties.kind = static_cast<entity::DamageKind>(kind);
  }
}

void DrawAbilityProperties(entity::AbilityProperties &properties,
                           const hierarchy::HierarchyViewModel &viewModel) {
  const char *currentType =
      std::holds_alternative<entity::HealingAbilityProperties>(
          properties.ability)
          ? "Healing"
          : "Damage";

  if (ImGui::BeginCombo("Ability kind", currentType)) {
    if (ImGui::Selectable("Healing")) {
      properties.ability = entity::HealingAbilityProperties{};
    }

    if (ImGui::Selectable("Damage")) {
      properties.ability = entity::DamageAbilityProperties{};
    }

    ImGui::EndCombo();
  }

  std::visit(
      [&viewModel](auto &value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, entity::HealingAbilityProperties>) {
          widgets::DrawUnsigned("Hit points", value.hitPoints);
        } else {
          widgets::DrawEntityReference("Damage type", value.damageType,
                                       entity::EntityType::DamageType,
                                       viewModel);
        }
      },
      properties.ability);
}

void DrawEffectProperties(entity::EffectProperties &properties) {
  const char *currentType = "Poisoning";

  if (std::holds_alternative<entity::DamageEffectProperties>(properties)) {
    currentType = "Damage";
  } else if (std::holds_alternative<entity::SlownessEffectProperties>(
                 properties)) {
    currentType = "Slowness";
  }

  if (ImGui::BeginCombo("Effect kind", currentType)) {
    if (ImGui::Selectable("Poisoning")) {
      properties = entity::PoisoningEffectProperties{};
    }

    if (ImGui::Selectable("Damage")) {
      properties = entity::DamageEffectProperties{};
    }

    if (ImGui::Selectable("Slowness")) {
      properties = entity::SlownessEffectProperties{};
    }

    ImGui::EndCombo();
  }

  std::visit(
      [](auto &value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, entity::PoisoningEffectProperties>) {
          widgets::DrawUnsigned("Damage", value.damage);
          widgets::DrawUnsigned("Poisoning duration", value.poisoningDuration);
          ImGui::InputDouble("Poisoning rate", &value.poisoningRate);
        } else if constexpr (std::is_same_v<T,
                                            entity::DamageEffectProperties>) {
          widgets::DrawUnsigned("Damage", value.damage);
        } else if constexpr (std::is_same_v<T,
                                            entity::SlownessEffectProperties>) {
          ImGui::InputDouble("Multiplier", &value.multiplier);
          widgets::DrawUnsigned("Duration", value.duration);
        }
      },
      properties);
}
} // namespace inspector::combat_properties