#pragma once

#include <cstdint>
#include <filesystem>

enum class EntityType {
  Image,
};

class Entity {
private:
  const uint32_t id;
  const EntityType type;
  const std::string name;

protected:
  Entity(uint32_t id, EntityType type, std::string name)
      : id(id), type(type), name(name) {}

public:
  uint32_t getId() const { return id; }
  EntityType getType() const { return type; }
  const std::string &getName() const { return name; }
};

// Texture stores:
// 1. File data
// 2. File type (".png")
class Image : Entity {
private:
  char *filetype;
  char *data;
  size_t data_size;

public:
  Image(uint32_t id, std::string name, std::filesystem::path imagepath);
};
