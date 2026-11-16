#include "editor/map_view.h"
#include "imgui.h"

namespace map_view {
void DrawGrid(ImDrawList *drawList, ImVec2 origin, ImVec2 size) {
  constexpr float gridStep = 32.0f;
  const ImU32 gridColor = IM_COL32(60, 100, 40, 255);

  for (float x = 0.0f; x <= size.x; x += gridStep) {
    drawList->AddLine(ImVec2(origin.x + x, origin.y),
                      ImVec2(origin.x + x, origin.y + size.y), gridColor, 1.0f);
  }
  for (float y = 0.0f; y <= size.y; y += gridStep) {
    drawList->AddLine(ImVec2(origin.x, origin.y + y),
                      ImVec2(origin.x + size.x, origin.y + y), gridColor, 1.0f);
  }
}
} 

void DrawMapView(float width, float height) {
  ImGui::BeginChild("MapView", ImVec2(width, height), true);

  const ImVec2 canvasOrigin = ImGui::GetCursorScreenPos();
  const ImVec2 canvasSize = ImGui::GetContentRegionAvail();
  ImDrawList *drawList = ImGui::GetWindowDrawList();

  drawList->AddRectFilled(
      canvasOrigin,
      ImVec2(canvasOrigin.x + canvasSize.x, canvasOrigin.y + canvasSize.y),
      IM_COL32(145, 205, 145, 255));

  map_view::DrawGrid(drawList, canvasOrigin, canvasSize);

  ImGui::Dummy(canvasSize);

  ImGui::EndChild();
}