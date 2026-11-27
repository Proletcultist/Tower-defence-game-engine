#include "editor/hierarchy_view_model.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace hierarchy {
HierarchyViewModel::HierarchyViewModel() { Add(entity::EntityType::Level); }

EntityId HierarchyViewModel::Add(entity::EntityType type) {
  if (type == entity::EntityType::None) {
    throw std::invalid_argument("Cannot create an entity with type None");
  }

  if (type == entity::EntityType::Level) {
    const auto levels = projectEntities_.find(entity::EntityType::Level);

    if (levels != projectEntities_.end() && !levels->second.empty()) {
      throw std::invalid_argument("A project can only have one Level for now");
    }
  }

  const std::string name =
      GetDefaultName(type) + "_" + std::to_string(++nextIndex_[type]);

  const EntityId entityId = nextEntityId_++;
  projectEntities_[type].push_back({entityId, type, name});

  if (type == entity::EntityType::Level) {
    const LevelNodeId rootNodeId = nextLevelNodeId_++;
    levelRoots_.push_back({rootNodeId, entityId, {}});
    openedLevelNodeId_ = rootNodeId;
  }

  return entityId;
}

bool HierarchyViewModel::AddToLevel(EntityId entityId,
                                    LevelNodeId parentNodeId) {
  const HierarchyItem *item = FindEntity(entityId);

  if (item == nullptr || item->type == entity::EntityType::Level) {
    return false;
  }

  LevelNode *parent = FindLevelNode(levelRoots_, parentNodeId);

  if (parent == nullptr) {
    return false;
  }

  parent->children.push_back({nextLevelNodeId_++, entityId, {}});
  return true;
}

bool HierarchyViewModel::IsInLevel(EntityId entityId) const {
  return ContainsEntity(levelRoots_, entityId);
}

bool HierarchyViewModel::OpenLevel(EntityId entityId) {
  const HierarchyItem *item = FindEntity(entityId);

  if (item == nullptr || item->type != entity::EntityType::Level) {
    return false;
  }

  const auto root = std::find_if(levelRoots_.begin(), levelRoots_.end(),
                                 [entityId](const LevelNode &candidate) {
                                   return candidate.entityId == entityId;
                                 });

  if (root == levelRoots_.end()) {
    return false;
  }

  openedLevelNodeId_ = root->id;
  return true;
}

bool HierarchyViewModel::CloseLevel(EntityId entityId) {
  if (!IsLevelOpen(entityId)) {
    return false;
  }

  openedLevelNodeId_.reset();

  if (selected_ && selected_->source == SelectionSource::Level) {
    selected_.reset();
  }

  return true;
}

bool HierarchyViewModel::IsLevelOpen(EntityId entityId) const {
  if (!openedLevelNodeId_) {
    return false;
  }

  const LevelNode *root = FindLevelNode(levelRoots_, *openedLevelNodeId_);

  return root != nullptr && root->entityId == entityId;
}

bool HierarchyViewModel::Remove(EntityId entityId) {
  for (auto category = projectEntities_.begin();
       category != projectEntities_.end(); ++category) {
    auto &items = category->second;

    const auto item = std::find_if(items.begin(), items.end(),
                                   [entityId](const HierarchyItem &candidate) {
                                     return candidate.id == entityId;
                                   });

    if (item == items.end()) {
      continue;
    }

    // Пока нельзя удалить единственный Level проекта.
    if (item->type == entity::EntityType::Level) {
      return false;
    }

    items.erase(item);

    if (items.empty()) {
      projectEntities_.erase(category);
    }

    RemoveEntityReferences(levelRoots_, entityId);

    if (selected_ && selected_->entityId == entityId) {
      selected_.reset();
    } else if (selected_ && selected_->source == SelectionSource::Level &&
               FindLevelNode(levelRoots_, selected_->levelNodeId) == nullptr) {
      selected_.reset();
    }

    return true;
  }

  return false;
}

bool HierarchyViewModel::RemoveFromLevel(LevelNodeId nodeId) {
  const LevelNode *node = FindLevelNode(levelRoots_, nodeId);

  if (node == nullptr) {
    return false;
  }

  const HierarchyItem *item = FindEntity(node->entityId);

  if (item != nullptr && item->type == entity::EntityType::Level) {
    return CloseLevel(item->id);
  }

  if (!RemoveLevelNode(levelRoots_, nodeId)) {
    return false;
  }

  if (selected_ && selected_->source == SelectionSource::Level &&
      FindLevelNode(levelRoots_, selected_->levelNodeId) == nullptr) {
    selected_.reset();
  }

  return true;
}

void HierarchyViewModel::SelectProjectEntity(EntityId entityId) {
  if (FindEntity(entityId) != nullptr) {
    selected_ = {entityId, 0, SelectionSource::Project};
  }
}

void HierarchyViewModel::SelectLevelNode(LevelNodeId nodeId) {
  const LevelNode *node = FindLevelNode(levelRoots_, nodeId);

  if (node != nullptr) {
    selected_ = {node->entityId, nodeId, SelectionSource::Level};
  }
}

const std::optional<HierarchySelection> &
HierarchyViewModel::GetSelected() const {
  return selected_;
}

const std::vector<HierarchyItem> *
HierarchyViewModel::FindProjectItems(entity::EntityType type) const {
  const auto category = projectEntities_.find(type);

  if (category == projectEntities_.end()) {
    return nullptr;
  }

  return &category->second;
}

const HierarchyItem *HierarchyViewModel::FindEntity(EntityId entityId) const {
  for (const auto &[type, items] : projectEntities_) {
    (void)type;

    const auto item = std::find_if(items.begin(), items.end(),
                                   [entityId](const HierarchyItem &candidate) {
                                     return candidate.id == entityId;
                                   });

    if (item != items.end()) {
      return &*item;
    }
  }

  return nullptr;
}

const LevelNode *HierarchyViewModel::GetOpenLevelRoot() const {
  if (!openedLevelNodeId_) {
    return nullptr;
  }

  return FindLevelNode(levelRoots_, *openedLevelNodeId_);
}

LevelNode *HierarchyViewModel::FindLevelNode(std::vector<LevelNode> &nodes,
                                             LevelNodeId nodeId) {
  for (LevelNode &node : nodes) {
    if (node.id == nodeId) {
      return &node;
    }

    if (LevelNode *found = FindLevelNode(node.children, nodeId)) {
      return found;
    }
  }

  return nullptr;
}

const LevelNode *
HierarchyViewModel::FindLevelNode(const std::vector<LevelNode> &nodes,
                                  LevelNodeId nodeId) {
  for (const LevelNode &node : nodes) {
    if (node.id == nodeId) {
      return &node;
    }

    if (const LevelNode *found = FindLevelNode(node.children, nodeId)) {
      return found;
    }
  }

  return nullptr;
}

bool HierarchyViewModel::ContainsEntity(const std::vector<LevelNode> &nodes,
                                        EntityId entityId) {
  for (const LevelNode &node : nodes) {
    if (node.entityId == entityId || ContainsEntity(node.children, entityId)) {
      return true;
    }
  }

  return false;
}

bool HierarchyViewModel::RemoveLevelNode(std::vector<LevelNode> &nodes,
                                         LevelNodeId nodeId) {
  const auto node = std::find_if(
      nodes.begin(), nodes.end(),
      [nodeId](const LevelNode &candidate) { return candidate.id == nodeId; });

  if (node != nodes.end()) {
    nodes.erase(node);
    return true;
  }

  for (LevelNode &parent : nodes) {
    if (RemoveLevelNode(parent.children, nodeId)) {
      return true;
    }
  }

  return false;
}

void HierarchyViewModel::RemoveEntityReferences(std::vector<LevelNode> &nodes,
                                                EntityId entityId) {
  nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
                             [entityId](const LevelNode &node) {
                               return node.entityId == entityId;
                             }),
              nodes.end());

  for (LevelNode &node : nodes) {
    RemoveEntityReferences(node.children, entityId);
  }
}

std::string HierarchyViewModel::GetDefaultName(entity::EntityType type) {
  switch (type) {
  case entity::EntityType::None:
    return {};
  case entity::EntityType::Level:
    return "Leveldefault";
  case entity::EntityType::GameMapBackground:
    return "GameMapBackgrounddefault";
  case entity::EntityType::GameMap:
    return "GameMapdefault";
  case entity::EntityType::Texture:
    return "Texturedefault";
  case entity::EntityType::EnemyType:
    return "EnemyTypedefault";
  case entity::EntityType::EnemyWave:
    return "EnemyWavedefault";
  case entity::EntityType::EnemyWaveOnPath:
    return "EnemyWaveOnPathdefault";
  case entity::EntityType::EnemyPath:
    return "EnemyPathdefault";
  case entity::EntityType::TowerType:
    return "TowerTypedefault";
  case entity::EntityType::TowerSpot:
    return "TowerSpotdefault";
  case entity::EntityType::DamageType:
    return "DamageTypedefault";
  case entity::EntityType::Ability:
    return "Abilitydefault";
  case entity::EntityType::Effect:
    return "Effectdefault";
  }

  return {};
}
} // namespace hierarchy