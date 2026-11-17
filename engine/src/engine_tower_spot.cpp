#include "engine/engine_tower_spot.h"

namespace engine {

bool EngineTowerSpot::hasTower() const noexcept { return towerType_ != nullptr; }

const TowerType *EngineTowerSpot::getTowerType() const noexcept {
  return towerType_;
}

void EngineTowerSpot::setTowerType(const TowerType *type) noexcept {
  towerType_ = type;
}

void EngineTowerSpot::removeTower() noexcept { towerType_ = nullptr; }

}
