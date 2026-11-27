#include "editor/inspector_map_properties.h"
#include "editor/inspector_reference_widgets.h"
#include "imgui.h"
#include <cstdio>
#include <optional>
#include <string>
#include <vector>

namespace inspector::map_properties {

static void
DrawTiles(std::vector<std::vector<std::optional<entity::EntityId>>> &tiles,
          unsigned int height, unsigned int width,
          const hierarchy::HierarchyViewModel &viewModel) {
  tiles.resize(height);
  for (auto &row : tiles) {
    row.resize(width);
  }

  if (!ImGui::TreeNode("Tiles")) {
    return;
  }

  for (std::size_t row = 0; row < tiles.size(); ++row) {
    ImGui::PushID(static_cast<int>(row));
    const std::string rowLabel = "Row " + std::to_string(row + 1);

    if (ImGui::TreeNode(rowLabel.c_str())) {
      for (std::size_t column = 0; column < tiles[row].size(); ++column) {
        ImGui::PushID(static_cast<int>(column));
        ImGui::Text("Tile %d", static_cast<int>(column + 1));
        widgets::DrawEntityReference("Texture", tiles[row][column],
                                     entity::EntityType::Texture, viewModel);
        ImGui::PopID();
      }
      ImGui::TreePop();
    }
    ImGui::PopID();
  }
  ImGui::TreePop();
}

void DrawGameMapBackgroundProperties(
    entity::GameMapBackgroundProperties &properties,
    const hierarchy::HierarchyViewModel &viewModel) {
  ImGui::InputDouble("Cell size", &properties.cellSize);
  widgets::DrawUnsigned("Height", properties.height);
  widgets::DrawUnsigned("Width", properties.width);
  DrawTiles(properties.tiles, properties.height, properties.width, viewModel);
}

void DrawGameMapProperties(entity::GameMapProperties &properties,
                           const hierarchy::HierarchyViewModel &viewModel) {
  widgets::DrawEntityReference("Background", properties.background,
                               entity::EntityType::GameMapBackground,
                               viewModel);
  widgets::DrawEntityReferenceList("Spots", properties.spots,
                                   entity::EntityType::TowerSpot, viewModel);
  widgets::DrawEntityReferenceList("Paths", properties.paths,
                                   entity::EntityType::EnemyPath, viewModel);
}

void DrawTextureProperties(entity::TextureProperties &properties) {
  char buffer[512]{};
  std::snprintf(buffer, sizeof(buffer), "%s", properties.imagePath.c_str());
  if (ImGui::InputText("Image path", buffer, sizeof(buffer))) {
    properties.imagePath = buffer;
  }
}

void DrawEnemyPathProperties(entity::EnemyPathProperties &properties) {
  widgets::DrawWaypoints(properties.waypoints);
}

} // namespace inspector::map_properties