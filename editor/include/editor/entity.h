#pragma once

#include "editor/entity_properties.h"
#include "editor/entity_type.h"

#include <string>

namespace entity {
struct Entity {
  EntityId id;
  EntityType type;
  std::string name;
  EntityProperties properties;
};
} // namespace entity