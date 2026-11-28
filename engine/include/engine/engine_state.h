#pragma once

#include <cstdint>
#include <vector>

#include "engine/game_state.h"

namespace engine {
// temporary struct, will be replaced by EngineTowerSpot later
struct TowerSpotData {
  uint32_t id = 0;
  float x = 0.0f;
  float y = 0.0f;
};

class EngineState {
public:
  GameState &getGameState() noexcept;             // for editing
  const GameState &getGameState() const noexcept; // readonly

  const std::vector<TowerSpotData> &getTowerSpots() const noexcept;
  void addTowerSpot(const TowerSpotData &spot);
  const TowerSpotData &getTowerSpot(uint32_t spotId) const;

private:
  GameState gameState_;
  std::vector<TowerSpotData> towerSpots_;
};
} // namespace engine