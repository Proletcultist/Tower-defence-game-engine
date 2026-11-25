#include "engine/test_level.h"

namespace engine {
void LoadTestLevel(EngineState &state) {
  state.getGameState() = GameState(100, 20);

  state.addTowerSpot({1, 150.0f, 150.0f});
  state.addTowerSpot({2, 300.0f, 300.0f});
  state.addTowerSpot({3, 450.0f, 150.0f});
}
} // namespace engine