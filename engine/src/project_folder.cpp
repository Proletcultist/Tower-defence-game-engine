#include "engine/project_folder.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <sys/wait.h>

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
  if (status != -1 && WIFEXITED(status) && WEXITSTATUS(status) == 1) {
    return {};
  }
  if (status != 0) {
    throw std::runtime_error("Could not open folder dialog (Zenity).");
  }
  if (!folder.empty() && folder.back() == '\n') {
    folder.pop_back();
  }
  if (folder.empty()) {
    return {};
  }

  const auto projectFile = std::filesystem::path(folder) / "project.tdg";
  std::error_code error;
  if (!std::filesystem::is_regular_file(projectFile, error)) {
    throw std::runtime_error("Selected folder must contain a project.tdg file.");
  }
  if (!std::ifstream(projectFile, std::ios::binary)) {
    throw std::runtime_error("Cannot read project.tdg.");
  }
  return folder;
}
