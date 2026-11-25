#include "engine/engine_state.h"
#include "engine/game_hud.h"
#include "engine/test_level.h"
#include "raylib.h"

int main() {
  InitWindow(800, 450, "TDE: Test Level");
  if (!IsWindowReady())
    return 1;

  SetTargetFPS(60);

  engine::EngineState state;
  engine::LoadTestLevel(state);

  engine::GameHUD hud;
  const engine::GameState &gameState = state.getGameState();
  hud.updateMoney(gameState.getMoney());
  hud.updateBaseHP(gameState.getBaseHP());
  hud.updateWave(gameState.getWaveNumber());

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (const engine::TowerSpotData &spot : state.getTowerSpots()) {
      DrawCircleV({spot.x, spot.y}, 20.0f, LIGHTGRAY);
      DrawCircleLinesV({spot.x, spot.y}, 20.0f, DARKGRAY);
    }

    hud.draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}