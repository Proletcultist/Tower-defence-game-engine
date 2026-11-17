#pragma once

namespace engine {

class TowerType;

class EngineTowerSpot {
public:
  bool hasTower() const noexcept;
  const TowerType *getTowerType() const noexcept;
  void setTowerType(const TowerType *type) noexcept;
  void removeTower() noexcept;

private:
  const TowerType *towerType_ = nullptr;
};

}
