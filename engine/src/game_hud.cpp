#include "engine/game_hud.h"

#include "raylib.h"

namespace engine {
constexpr int margin = 10;
constexpr int fontSize = 20;
constexpr int lineHeight = 24;

void GameHUD::updateMoney(int32_t money) noexcept { displayedMoney_ = money; }
void GameHUD::updateBaseHP(int32_t baseHP) noexcept {
  displayedBaseHP_ = baseHP;
}
void GameHUD::updateWave(uint16_t waveNumber) noexcept {
  displayedWave_ = waveNumber;
}

void GameHUD::draw() const {
  DrawText(TextFormat("Money: %d", displayedMoney_), margin, margin, fontSize,
           GOLD);
  DrawText(TextFormat("Base HP: %d", displayedBaseHP_), margin,
           margin + lineHeight, fontSize, MAROON);
  DrawText(TextFormat("Wave: %u", displayedWave_), margin,
           margin + 2 * lineHeight, fontSize, BLUE);
}
} // namespace engine