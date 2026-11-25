#pragma once

#include <variant>
#include <system_error>

class DirectoryDoesntExist {};
class FilesystemError {
  private:
    const std::error_code code;
  public:

    FilesystemError(std::error_code code) : code(code) {}
    std::error_code getCode() const { return code; }
};
class NonEmptyProjectDirectory {};
class NotADirectory {};

typedef std::variant<DirectoryDoesntExist, FilesystemError, NonEmptyProjectDirectory, NotADirectory> GameRepositoryCreationError;
