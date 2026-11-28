#pragma once

#include <cstdint>

namespace engine {
class GameState {
public:
  GameState() = default;
  GameState(int32_t money, int32_t baseHP);

  int32_t getMoney() const noexcept;
  void setMoney(int32_t money) noexcept;

  int32_t getBaseHP() const noexcept;
  void setBaseHP(int32_t baseHP) noexcept;

  uint16_t getWaveNumber() const noexcept;
  void setWaveNumber(uint16_t waveNumber) noexcept;

  double getGameSpeedMultiplier() const noexcept;
  void setGameSpeedMultiplier(double multiplier);

private:
  int32_t money_ = 0;
  int32_t baseHP_ = 0;
  uint16_t waveNumber_ = 0;
  double gameSpeedMultiplier_ = 1.0;
};
} // namespace engine