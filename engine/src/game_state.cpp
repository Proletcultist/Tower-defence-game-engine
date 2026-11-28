#include "engine/game_state.h"

#include <stdexcept>

namespace engine {
GameState::GameState(int32_t money, int32_t baseHP)
    : money_(money), baseHP_(baseHP) {}

int32_t GameState::getMoney() const noexcept { return money_; }
void GameState::setMoney(int32_t money) noexcept { money_ = money; }

int32_t GameState::getBaseHP() const noexcept { return baseHP_; }
void GameState::setBaseHP(int32_t baseHP) noexcept { baseHP_ = baseHP; }

uint16_t GameState::getWaveNumber() const noexcept { return waveNumber_; }
void GameState::setWaveNumber(uint16_t waveNumber) noexcept {
  waveNumber_ = waveNumber;
}

double GameState::getGameSpeedMultiplier() const noexcept {
  return gameSpeedMultiplier_;
}
void GameState::setGameSpeedMultiplier(double multiplier) {
  if (!(multiplier > 0.0))
    throw std::invalid_argument("Game speed multiplier must be >0.0");

  gameSpeedMultiplier_ = multiplier;
}
} // namespace engine