#include <engine/engine_state.h>

#include <algorithm>
#include <stdexcept>
#include <string>

namespace engine {
GameState &EngineState::getGameState() noexcept { return gameState_; }
const GameState &EngineState::getGameState() const noexcept {
  return gameState_;
}

const std::vector<TowerSpotData> &EngineState::getTowerSpots() const noexcept {
  return towerSpots_;
}

void EngineState::addTowerSpot(const TowerSpotData &spot) {
  const auto sameId = [&spot](const TowerSpotData &other) {
    return other.id == spot.id;
  };

  if (std::any_of(towerSpots_.begin(), towerSpots_.end(), sameId))
    throw std::invalid_argument("Tower spot " + std::to_string(spot.id) +
                                " already exists.");

  towerSpots_.push_back(spot);
}

const TowerSpotData &EngineState::getTowerSpot(uint32_t spotId) const {
  const auto it = std::find_if(
      towerSpots_.begin(), towerSpots_.end(),
      [spotId](const TowerSpotData &spot) { return spot.id == spotId; });

  if (it == towerSpots_.end())
    throw std::out_of_range("Tower spot " + std::to_string(spotId) +
                            " does not exist.");

  return *it;
}
} // namespace engine