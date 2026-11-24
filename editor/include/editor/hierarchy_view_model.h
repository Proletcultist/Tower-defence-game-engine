#pragma once

#include "editor/entity_creation_popup.h"

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace hierarchy {
struct HierarchyItem {
  std::string name;
};

struct HierarchySelection {
  entity_creation_popup::EntityType type;
  std::string name;
};

class HierarchyViewModel {
public:
  void Add(entity_creation_popup::EntityType type);

  bool Remove(entity_creation_popup::EntityType type, const std::string &name);

  void Select(entity_creation_popup::EntityType type, const std::string &name);

  const std::optional<HierarchySelection> &GetSelected() const;

  const std::vector<HierarchyItem> *
  FindItems(entity_creation_popup::EntityType type) const;

private:
  std::map<entity_creation_popup::EntityType, std::vector<HierarchyItem>>
      items_;

  std::map<entity_creation_popup::EntityType, std::size_t> nextIndex_;
  std::optional<HierarchySelection> selected_;

  static std::string GetDefaultName(entity_creation_popup::EntityType type);
};
} // namespace hierarchy