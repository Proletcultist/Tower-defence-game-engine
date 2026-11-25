#include "editor/hierarchy_view_model.h"

#include <algorithm>

namespace hierarchy {
void HierarchyViewModel::Add(entity::EntityType type) {
  if (type == entity::EntityType::None) {
    return;
  }

  const std::string name =
      GetDefaultName(type) + "_" + std::to_string(++nextIndex_[type]);

  items_[type].push_back({name});
}

bool HierarchyViewModel::Remove(entity::EntityType type,
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

void HierarchyViewModel::Select(entity::EntityType type,
                                const std::string &name) {
  selected_ = HierarchySelection{type, name};
}

const std::optional<HierarchySelection> &
HierarchyViewModel::GetSelected() const {
  return selected_;
}

const std::vector<HierarchyItem> *
HierarchyViewModel::FindItems(entity::EntityType type) const {
  const auto it = items_.find(type);

  if (it == items_.end()) {
    return nullptr;
  }

  return &it->second;
}

std::string HierarchyViewModel::GetDefaultName(entity::EntityType type) {
  switch (type) {
  case entity::EntityType::GameMapBackground:
    return "GameMapBackgrounddefault";

  case entity::EntityType::GameMap:
    return "GameMapdefault";

  case entity::EntityType::Texture:
    return "Texturedefault";

  case entity::EntityType::EnemyType:
    return "EnemyTypedefault";

  case entity::EntityType::EnemyWave:
    return "EnemyWavedefault";

  case entity::EntityType::EnemyWaveOnPath:
    return "EnemyWaveOnPathdefault";

  case entity::EntityType::EnemyPath:
    return "EnemyPathdefault";

  case entity::EntityType::TowerType:
    return "TowerTypedefault";

  case entity::EntityType::TowerSpot:
    return "TowerSpotdefault";

  case entity::EntityType::DamageType:
    return "DamageTypedefault";

  case entity::EntityType::Ability:
    return "Abilitydefault";

  case entity::EntityType::Effect:
    return "Effectdefault";

  case entity::EntityType::None:
    return {};
  }

  return {};
}
} // namespace hierarchy