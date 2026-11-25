#pragma once

#include "entity_type.h"

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace hierarchy {
struct HierarchyItem {
  std::string name;
};

struct HierarchySelection {
  entity::EntityType type;
  std::string name;
};

class HierarchyViewModel {
public:
  void Add(entity::EntityType type);

  bool Remove(entity::EntityType type, const std::string &name);

  void Select(entity::EntityType type, const std::string &name);

  const std::optional<HierarchySelection> &GetSelected() const;

  const std::vector<HierarchyItem> *
  FindItems(entity::EntityType type) const;

private:
  std::map<entity::EntityType, std::vector<HierarchyItem>>
      items_;

  std::map<entity::EntityType, std::size_t> nextIndex_;
  std::optional<HierarchySelection> selected_;

  static std::string GetDefaultName(entity::EntityType type);
};
} // namespace hierarchy