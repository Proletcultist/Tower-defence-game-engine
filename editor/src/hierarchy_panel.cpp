#include "editor/hierarchy_panel.h"

#include "editor/entity_creation_popup.h"

#include "imgui.h"

namespace hierarchy {
static void DrawCategory(HierarchyViewModel &viewModel,
                         const char *categoryName, entity::EntityType type) {
  const auto *items = viewModel.FindProjectItems(type);

  if (items == nullptr || items->empty()) {
    return;
  }

  if (!ImGui::TreeNodeEx(categoryName)) {
    return;
  }

  EntityId idToDelete = 0;
  bool shouldDelete = false;

  for (const HierarchyItem &item : *items) {
    const auto &selected = viewModel.GetSelected();

    const bool isSelected = selected &&
                            selected->source == SelectionSource::Project &&
                            selected->entityId == item.id;

    ImGui::PushID(static_cast<int>(item.id));

    if (ImGui::Selectable(item.name.c_str(), isSelected)) {
      viewModel.SelectProjectEntity(item.id);
    }

    if (ImGui::BeginPopupContextItem("ProjectEntityContextMenu")) {
      if (item.type == entity::EntityType::Level) {
        if (viewModel.IsLevelOpen(item.id)) {
          if (ImGui::MenuItem("Close Level")) {
            viewModel.CloseLevel(item.id);
          }
        } else if (ImGui::MenuItem("Open Level")) {
          viewModel.OpenLevel(item.id);
        }
      } else if (ImGui::MenuItem("Delete")) {
        idToDelete = item.id;
        shouldDelete = true;
      }

      ImGui::EndPopup();
    }

    ImGui::PopID();
  }

  ImGui::TreePop();

  if (shouldDelete) {
    viewModel.Remove(idToDelete);
  }
}

static void DrawProjectDefinitions(HierarchyViewModel &viewModel) {
  if (!ImGui::CollapsingHeader("PROJECT DEFINITIONS",
                               ImGuiTreeNodeFlags_DefaultOpen)) {
    return;
  }

  DrawCategory(viewModel, "Levels", entity::EntityType::Level);
  DrawCategory(viewModel, "Game Map Backgrounds",
               entity::EntityType::GameMapBackground);
  DrawCategory(viewModel, "Game Maps", entity::EntityType::GameMap);
  DrawCategory(viewModel, "Textures", entity::EntityType::Texture);
  DrawCategory(viewModel, "Enemies", entity::EntityType::EnemyType);
  DrawCategory(viewModel, "Enemy Waves", entity::EntityType::EnemyWave);
  DrawCategory(viewModel, "Enemy Waves On Paths",
               entity::EntityType::EnemyWaveOnPath);
  DrawCategory(viewModel, "Enemy Paths", entity::EntityType::EnemyPath);
  DrawCategory(viewModel, "Towers", entity::EntityType::TowerType);
  DrawCategory(viewModel, "Tower Spots", entity::EntityType::TowerSpot);
  DrawCategory(viewModel, "Damage Types", entity::EntityType::DamageType);
  DrawCategory(viewModel, "Abilities", entity::EntityType::Ability);
  DrawCategory(viewModel, "Effects", entity::EntityType::Effect);
}

static void DrawParentOptions(HierarchyViewModel &viewModel,
                              const LevelNode &node, bool &addRequested,
                              LevelNodeId &chosenParentId) {
  const HierarchyItem *item = viewModel.FindEntity(node.entityId);

  if (item == nullptr) {
    return;
  }

  ImGui::PushID(static_cast<int>(node.id));

  if (ImGui::BeginMenu(item->name.c_str())) {
    if (ImGui::MenuItem("Add as child here")) {
      chosenParentId = node.id;
      addRequested = true;
    }

    for (const LevelNode &child : node.children) {
      DrawParentOptions(viewModel, child, addRequested, chosenParentId);
    }

    ImGui::EndMenu();
  }

  ImGui::PopID();
}

static void DrawAddToLevel(HierarchyViewModel &viewModel) {
  const auto &selected = viewModel.GetSelected();

  if (!selected || selected->source != SelectionSource::Project) {
    return;
  }

  const HierarchyItem *selectedItem = viewModel.FindEntity(selected->entityId);

  if (selectedItem == nullptr ||
      selectedItem->type == entity::EntityType::Level) {
    return;
  }

  if (ImGui::Button("Add selected to level")) {
    ImGui::OpenPopup("ChooseLevelParentPopup");
  }

  bool addRequested = false;
  LevelNodeId chosenParentId = 0;

  if (ImGui::BeginPopup("ChooseLevelParentPopup")) {
    const LevelNode *root = viewModel.GetOpenLevelRoot();

    if (root == nullptr) {
      ImGui::TextWrapped("Open a level before adding entities to it.");
    } else {
      DrawParentOptions(viewModel, *root, addRequested, chosenParentId);
    }

    ImGui::EndPopup();
  }

  if (addRequested && !viewModel.AddToLevel(selectedItem->id, chosenParentId)) {
    ImGui::TextWrapped("Could not add the entity to the selected parent.");
  }
}

static void DrawLevelNode(HierarchyViewModel &viewModel, const LevelNode &node,
                          LevelNodeId &nodeIdToRemove) {
  const HierarchyItem *item = viewModel.FindEntity(node.entityId);

  if (item == nullptr) {
    return;
  }

  const auto &selected = viewModel.GetSelected();

  const bool isSelected = selected &&
                          selected->source == SelectionSource::Level &&
                          selected->levelNodeId == node.id;

  ImGui::PushID(static_cast<int>(node.id));

  const bool hasChildren = !node.children.empty();

  const ImGuiTreeNodeFlags flags =
      (isSelected ? ImGuiTreeNodeFlags_Selected : 0) |
      (hasChildren
           ? 0
           : ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);

  const bool isOpen = ImGui::TreeNodeEx(item->name.c_str(), flags);

  if (ImGui::IsItemClicked()) {
    viewModel.SelectLevelNode(node.id);
  }

  if (ImGui::BeginPopupContextItem("LevelEntityContextMenu")) {
    if (item->type == entity::EntityType::Level) {
      if (ImGui::MenuItem("Close Level")) {
        nodeIdToRemove = node.id;
      }
    } else if (ImGui::MenuItem("Remove from level")) {
      nodeIdToRemove = node.id;
    }

    ImGui::EndPopup();
  }

  if (hasChildren && isOpen) {
    for (const LevelNode &child : node.children) {
      DrawLevelNode(viewModel, child, nodeIdToRemove);
    }

    ImGui::TreePop();
  }

  ImGui::PopID();
}

static void DrawLevelTree(HierarchyViewModel &viewModel) {
  if (!ImGui::CollapsingHeader("LEVEL TREE", ImGuiTreeNodeFlags_DefaultOpen)) {
    return;
  }

  LevelNodeId nodeIdToRemove = 0;
  const LevelNode *root = viewModel.GetOpenLevelRoot();

  if (root == nullptr) {
    ImGui::TextWrapped(
        "No level is open. Open a level from Project Definitions.");
  } else {
    DrawLevelNode(viewModel, *root, nodeIdToRemove);
  }

  if (nodeIdToRemove != 0) {
    viewModel.RemoveFromLevel(nodeIdToRemove);
  }
}

void DrawHierarchyPanel(float width, float height,
                        HierarchyViewModel &viewModel) {
  ImGui::BeginChild("HierarchyPanel", ImVec2(width, height), true);

  ImGui::TextUnformatted("HIERARCHY");

  constexpr float addButtonWidth = 22.0f;
  constexpr float rightPadding = 6.0f;

  ImGui::SameLine(ImGui::GetWindowWidth() - addButtonWidth - rightPadding);

  if (ImGui::Button("+", ImVec2(addButtonWidth, 0.0f))) {
    entity_creation_popup::OpenPopup();
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Create entity");
  }

  ImGui::Separator();

  const entity::EntityType newType = entity_creation_popup::DrawPopup();

  if (newType != entity::EntityType::None &&
      newType != entity::EntityType::Level) {
    viewModel.Add(newType);
  }

  DrawProjectDefinitions(viewModel);
  ImGui::Spacing();
  DrawAddToLevel(viewModel);
  ImGui::Spacing();
  DrawLevelTree(viewModel);

  if (!ImGui::GetIO().WantTextInput && ImGui::IsKeyPressed(ImGuiKey_Delete)) {
    const auto &selected = viewModel.GetSelected();

    if (selected) {
      if (selected->source == SelectionSource::Project) {
        viewModel.Remove(selected->entityId);
      } else {
        const HierarchyItem *item = viewModel.FindEntity(selected->entityId);

        if (item != nullptr && item->type == entity::EntityType::Level) {
          viewModel.CloseLevel(selected->entityId);
        } else {
          viewModel.RemoveFromLevel(selected->levelNodeId);
        }
      }
    }
  }

  ImGui::EndChild();
}
} // namespace hierarchy