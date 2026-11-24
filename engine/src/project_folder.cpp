#include "engine/project_folder.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <sys/wait.h>

namespace project_folder {

constexpr char projectFileName[] = "project.tdg";

std::string SelectProjectFolder() {
  FILE *dialog = popen(
      "zenity --file-selection --directory --title='Open Folder'", "r");
  if (!dialog) {
    throw std::runtime_error("Could not open folder dialog (Zenity).");
  }

  std::string folder;
  char buffer[4096];
  while (fgets(buffer, sizeof(buffer), dialog)) {
    folder += buffer;
  }

  const int status = pclose(dialog);
  switch (status != -1 && WIFEXITED(status) ? WEXITSTATUS(status) : -1) {
  case 0:
    break;
  case 1:
    return {};
  case -1:
    throw std::runtime_error("Could not wait for Zenity.");
  case 5:
    throw std::runtime_error("Zenity dialog timed out.");
  case 127:
    throw std::runtime_error("Zenity is not installed.");
  default:
    throw std::runtime_error("Zenity failed.");
  }
  if (!folder.empty() && folder.back() == '\n') {
    folder.pop_back();
  }
  if (folder.empty()) {
    throw std::runtime_error("Zenity returned an empty folder path.");
  }

  const auto projectFile = std::filesystem::path(folder) / projectFileName;
  std::error_code error;
  const bool isRegularFile = std::filesystem::is_regular_file(projectFile, error);
  if (error) {
    throw std::runtime_error("Could not check project file: " + error.message());
  }
  if (!isRegularFile) {
    throw std::runtime_error("Selected folder must contain a project.tdg file.");
  }
  if (!std::ifstream(projectFile, std::ios::binary)) {
    throw std::runtime_error("Cannot read project.tdg.");
  }
  return folder;
}

} // namespace project_folder
