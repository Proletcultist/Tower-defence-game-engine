#include <chrono>
#include <exception>
#include <future>
#include <string>

#include "engine/project_folder.h"
#include "raylib.h"

int main() {
  InitWindow(800, 450, "Tower Defence Engine");
  if (!IsWindowReady()) {
    return 1;
  }
  SetTargetFPS(60);

  const Rectangle openButton{20.0f, 20.0f, 220.0f, 45.0f};
  std::future<std::string> folderDialog;
  std::string projectDirectory;
  std::string message;
  bool hasError = false;

  while (!WindowShouldClose()) {
    if (folderDialog.valid() &&
        folderDialog.wait_for(std::chrono::seconds(0)) ==
            std::future_status::ready) {
      try {
        const auto folder = folderDialog.get();
        if (!folder.empty()) {
          projectDirectory = folder;
          const auto title = "Tower Defence Engine - " + projectDirectory;
          SetWindowTitle(title.c_str());
          message = "Project opened.";
        } else {
          message = "Folder selection cancelled.";
        }
      } catch (const std::exception &error) {
        message = "Error: " + std::string(error.what());
        hasError = true;
      }
    }

    const bool hovered = CheckCollisionPointRec(GetMousePosition(), openButton);
    if (!folderDialog.valid() && hovered &&
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      message.clear();
      hasError = false;
      folderDialog = std::async(std::launch::async, SelectProjectFolder);
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangleRec(openButton,
                     !folderDialog.valid() && hovered ? SKYBLUE : LIGHTGRAY);
    DrawText(folderDialog.valid() ? "Choosing folder..." : "Open Folder", 32,
             33, 20, DARKGRAY);
    DrawText(message.c_str(), 20, 120, 20, hasError ? RED : DARKGRAY);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
