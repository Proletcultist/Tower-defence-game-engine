#include "editor/hierarchy_view_model.h"

namespace hierarchy {
void HierarchyViewModel::Add(entity_creation_popup::EntityType type) {
  auto &items = items_[type];

  HierarchyItem item;
  item.name = GetDefaultName(type) + "_" + std::to_string(items.size() + 1);

  items.push_back(std::move(item));
}

const std::vector<HierarchyItem> *
HierarchyViewModel::FindItems(entity_creation_popup::EntityType type) const {
  const auto it = items_.find(type);

  if (it == items_.end()) {
    return nullptr;
  }

  return &it->second;
}

std::string
HierarchyViewModel::GetDefaultName(entity_creation_popup::EntityType type) {
  switch (type) {
  case entity_creation_popup::EntityType::GameMapBackground:
    return "GameMapBackgrounddefault";

  case entity_creation_popup::EntityType::GameMap:
    return "GameMapdefault";

  case entity_creation_popup::EntityType::Texture:
    return "Texturedefault";

  case entity_creation_popup::EntityType::EnemyType:
    return "EnemyTypedefault";

  case entity_creation_popup::EntityType::EnemyWave:
    return "EnemyWavedefault";

  case entity_creation_popup::EntityType::EnemyWaveOnPath:
    return "EnemyWaveOnPathdefault";

  case entity_creation_popup::EntityType::EnemyPath:
    return "EnemyPathdefault";

  case entity_creation_popup::EntityType::TowerType:
    return "TowerTypedefault";

  case entity_creation_popup::EntityType::TowerSpot:
    return "TowerSpotdefault";

  case entity_creation_popup::EntityType::DamageType:
    return "DamageTypedefault";

  case entity_creation_popup::EntityType::Ability:
    return "Abilitydefault";

  case entity_creation_popup::EntityType::Effect:
    return "Effectdefault";

  default:
    return {};
  }
}
} // namespace hierarchy