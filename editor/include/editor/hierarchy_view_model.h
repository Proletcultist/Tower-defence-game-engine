#pragma once

#include "editor/entity_creation_popup.h"

#include <map>
#include <string>
#include <vector>

namespace hierarchy {
struct HierarchyItem {
  std::string name;
};

class HierarchyViewModel {
public:
  void Add(entity_creation_popup::EntityType type);

  const std::vector<HierarchyItem> *
  FindItems(entity_creation_popup::EntityType type) const;

private:
  std::map<entity_creation_popup::EntityType, std::vector<HierarchyItem>>
      items_;

  static std::string GetDefaultName(entity_creation_popup::EntityType type);
};
} // namespace hierarchy