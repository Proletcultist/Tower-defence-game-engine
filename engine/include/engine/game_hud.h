#pragma once

#include <cstdint>

namespace engine {
class GameHUD {
public:
  void updateMoney(int32_t money) noexcept;
  void updateBaseHP(int32_t baseHP) noexcept;
  void updateWave(uint16_t waveNumber) noexcept;

  void draw() const;

private:
  int32_t displayedMoney_ = 0;
  int32_t displayedBaseHP_ = 0;
  uint16_t displayedWave_ = 0;
};
} // namespace engine