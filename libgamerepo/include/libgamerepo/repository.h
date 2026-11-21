#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>

#include "gamerepo/entity.h"

// What we need:
// - Read entity and all of its' components by id
// - Find candidates for the relation
// - Find all entities of some type
// - Write entities (with bufferization + flushing)

// Entities bson structure:
// {
//   "next_id": uint32_t = <next_available_id>,
//   "entities": [
//     <entity_objects...>
//   ],
// }

class GameRepository {
private:
  // buffer[id] == None means that entity was removed from repo
  // and when we implement more effective repository management,
  // it will mean that we need to remove an entity while flushing.
  std::unordered_map<uint32_t, std::optional<Entity>> buffer;

  std::filesystem::path root_path;

public:
  static EntitiesRepository open(std::filesystem::path root_path);
  static EntitiesRepository create(std::filesystem::path root_path);

  Entity readEntity(uint32_t id);
  Entity createEntity();
  void removeEntity(uint32_t id);
  void writeEntity(Entity entity);

  void flush();
};
