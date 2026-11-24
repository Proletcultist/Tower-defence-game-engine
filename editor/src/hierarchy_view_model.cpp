#include "editor/hierarchy_view_model.h"

#include <algorithm>

namespace hierarchy {
void HierarchyViewModel::Add(entity_creation_popup::EntityType type) {
  if (type == entity_creation_popup::EntityType::None) {
    return;
  }

  const std::string name =
      GetDefaultName(type) + "_" + std::to_string(++nextIndex_[type]);

  items_[type].push_back({name});
}

bool HierarchyViewModel::Remove(entity_creation_popup::EntityType type,
                                const std::string &name) {
  const auto category = items_.find(type);

  if (category == items_.end()) {
    return false;
  }

  const auto item =
      std::find_if(category->second.begin(), category->second.end(),
                   [&name](const HierarchyItem &candidate) {
                     return candidate.name == name;
                   });

  if (item == category->second.end()) {
    return false;
  }

  category->second.erase(item);

  if (category->second.empty()) {
    items_.erase(category);
  }

  if (selected_ && selected_->type == type && selected_->name == name) {
    selected_.reset();
  }

  return true;
}

void HierarchyViewModel::Select(entity_creation_popup::EntityType type,
                                const std::string &name) {
  selected_ = HierarchySelection{type, name};
}

const std::optional<HierarchySelection> &
HierarchyViewModel::GetSelected() const {
  return selected_;
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

  case entity_creation_popup::EntityType::None:
    return {};
  }

  return {};
}
} // namespace hierarchy