#pragma once

#include "editor/entity.h"

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace hierarchy {
using EntityId = entity::EntityId;
using LevelNodeId = std::size_t;
using HierarchyItem = entity::Entity;

struct LevelNode {
  LevelNodeId id;
  EntityId entityId;
  std::vector<LevelNode> children;
};

enum class SelectionSource { Project, Level };

struct HierarchySelection {
  EntityId entityId;
  LevelNodeId levelNodeId;
  SelectionSource source;
};

class HierarchyViewModel {
public:
  HierarchyViewModel();

  EntityId Add(entity::EntityType type);

  bool AddToLevel(EntityId entityId, LevelNodeId parentNodeId);
  bool IsInLevel(EntityId entityId) const;

  bool OpenLevel(EntityId entityId);
  bool CloseLevel(EntityId entityId);
  bool IsLevelOpen(EntityId entityId) const;

  bool Remove(EntityId entityId);
  bool RemoveFromLevel(LevelNodeId nodeId);

  void SelectProjectEntity(EntityId entityId);
  void SelectLevelNode(LevelNodeId nodeId);

  const std::optional<HierarchySelection> &GetSelected() const;

  const std::vector<HierarchyItem> *
  FindProjectItems(entity::EntityType type) const;

  HierarchyItem *FindEntity(EntityId entityId);
  const HierarchyItem *FindEntity(EntityId entityId) const;

  const LevelNode *GetOpenLevelRoot() const;

private:
  std::map<entity::EntityType, std::vector<HierarchyItem>> projectEntities_;
  std::vector<LevelNode> levelRoots_;

  std::map<entity::EntityType, std::size_t> nextIndex_;
  EntityId nextEntityId_ = 0;
  LevelNodeId nextLevelNodeId_ = 1;
  std::optional<LevelNodeId> openedLevelNodeId_;
  std::optional<HierarchySelection> selected_;

  static LevelNode *FindLevelNode(std::vector<LevelNode> &nodes,
                                  LevelNodeId nodeId);

  static const LevelNode *FindLevelNode(const std::vector<LevelNode> &nodes,
                                        LevelNodeId nodeId);

  static bool ContainsEntity(const std::vector<LevelNode> &nodes,
                             EntityId entityId);

  static bool RemoveLevelNode(std::vector<LevelNode> &nodes,
                              LevelNodeId nodeId);

  static void RemoveEntityReferences(std::vector<LevelNode> &nodes,
                                     EntityId entityId);

  static std::string GetDefaultName(entity::EntityType type);
};
} // namespace hierarchy