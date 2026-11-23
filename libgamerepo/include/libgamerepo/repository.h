#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>

#include "bsoncxx/document/value.hpp"
#include "libgamerepo/entity.h"

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
    std::unordered_map<uint32_t, bsoncxx::document::value> buffer;

    std::filesystem::path root_path;

  public:
    static GameRepository open(std::filesystem::path root_path);
    static GameRepository create(std::filesystem::path root_path);

    Entity readEntity(uint32_t id);
    Entity createEntity();
    void removeEntity(uint32_t id);
    void writeEntity(Entity entity);

    void flush();
};
