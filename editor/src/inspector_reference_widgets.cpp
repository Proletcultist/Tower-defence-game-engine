#include "editor/inspector_reference_widgets.h"

#include "imgui.h"

namespace inspector::widgets {
namespace {
const char *FindEntityName(const hierarchy::HierarchyViewModel &viewModel,
                           entity::EntityType type, entity::EntityId id) {
  const auto *items = viewModel.FindProjectItems(type);

  if (items != nullptr) {
    for (const hierarchy::HierarchyItem &item : *items) {
      if (item.id == id) {
        return item.name.c_str();
      }
    }
  }

  return "Missing entity";
}
} // namespace

void DrawUnsigned(const char *label, unsigned int &value) {
  ImGui::InputScalar(label, ImGuiDataType_U32, &value);
}

bool DrawEntityReference(const char *label,
                         std::optional<entity::EntityId> &selectedId,
                         entity::EntityType targetType,
                         const hierarchy::HierarchyViewModel &viewModel,
                         bool allowNone) {
  const auto *items = viewModel.FindProjectItems(targetType);

  const char *preview = selectedId
                            ? FindEntityName(viewModel, targetType, *selectedId)
                            : (allowNone ? "None" : "Select entity");

  if (!ImGui::BeginCombo(label, preview)) {
    return false;
  }

  bool changed = false;

  if (allowNone) {
    const bool isNoneSelected = !selectedId;

    if (ImGui::Selectable("None", isNoneSelected) && selectedId) {
      selectedId.reset();
      changed = true;
    }

    if (isNoneSelected) {
      ImGui::SetItemDefaultFocus();
    }
  }

  if (items != nullptr) {
    for (const hierarchy::HierarchyItem &item : *items) {
      ImGui::PushID(static_cast<int>(item.id));

      const bool isSelected = selectedId && *selectedId == item.id;

      if (ImGui::Selectable(item.name.c_str(), isSelected)) {
        if (!selectedId || *selectedId != item.id) {
          selectedId = item.id;
          changed = true;
        }
      }

      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }

      ImGui::PopID();
    }
  }

  ImGui::EndCombo();
  return changed;
}

void DrawEntityReferenceList(const char *label,
                             std::vector<entity::EntityId> &ids,
                             entity::EntityType targetType,
                             const hierarchy::HierarchyViewModel &viewModel) {
  if (!ImGui::TreeNode(label)) {
    return;
  }

  int indexToRemove = -1;

  for (std::size_t index = 0; index < ids.size(); ++index) {
    ImGui::PushID(static_cast<int>(index));

    std::optional<entity::EntityId> selectedId = ids[index];

    DrawEntityReference("Entity", selectedId, targetType, viewModel);

    if (!selectedId) {
      indexToRemove = static_cast<int>(index);
    } else {
      ids[index] = *selectedId;
    }

    ImGui::SameLine();

    if (ImGui::SmallButton("Remove")) {
      indexToRemove = static_cast<int>(index);
    }

    ImGui::PopID();
  }

  if (indexToRemove >= 0) {
    ids.erase(ids.begin() + indexToRemove);
  }

  ImGui::PushID(label);
  std::optional<entity::EntityId> newId;

  if (DrawEntityReference("Add entity", newId, targetType, viewModel, false) &&
      newId) {
    ids.push_back(*newId);
  }

  ImGui::PopID();
  ImGui::TreePop();
}

void DrawEntityReferencePairs(
    const char *label,
    std::vector<std::pair<unsigned int, entity::EntityId>> &values,
    const char *numberLabel, entity::EntityType targetType,
    const hierarchy::HierarchyViewModel &viewModel) {
  if (!ImGui::TreeNode(label)) {
    return;
  }

  int indexToRemove = -1;

  for (std::size_t index = 0; index < values.size(); ++index) {
    ImGui::PushID(static_cast<int>(index));

    DrawUnsigned(numberLabel, values[index].first);

    std::optional<entity::EntityId> selectedId = values[index].second;

    DrawEntityReference("Entity", selectedId, targetType, viewModel, false);

    if (selectedId) {
      values[index].second = *selectedId;
    }

    if (ImGui::SmallButton("Remove")) {
      indexToRemove = static_cast<int>(index);
    }

    ImGui::Separator();
    ImGui::PopID();
  }

  if (indexToRemove >= 0) {
    values.erase(values.begin() + indexToRemove);
  }

  ImGui::PushID(label);
  std::optional<entity::EntityId> newId;

  if (DrawEntityReference("Add entity", newId, targetType, viewModel, false) &&
      newId) {
    values.emplace_back(0, *newId);
  }

  ImGui::PopID();
  ImGui::TreePop();
}

void DrawEffectMultipliers(std::map<entity::EntityId, double> &multipliers,
                           const hierarchy::HierarchyViewModel &viewModel) {
  if (!ImGui::TreeNode("Effect multipliers")) {
    return;
  }

  std::optional<entity::EntityId> idToRemove;

  for (auto &[effectId, multiplier] : multipliers) {
    ImGui::PushID(static_cast<int>(effectId));

    ImGui::Text(
        "Effect: %s",
        FindEntityName(viewModel, entity::EntityType::Effect, effectId));
    ImGui::InputDouble("Multiplier", &multiplier);

    if (ImGui::SmallButton("Remove")) {
      idToRemove = effectId;
    }

    ImGui::Separator();
    ImGui::PopID();
  }

  if (idToRemove) {
    multipliers.erase(*idToRemove);
  }

  ImGui::PushID("AddEffectMultiplier");
  std::optional<entity::EntityId> newEffectId;

  if (DrawEntityReference("Add effect", newEffectId, entity::EntityType::Effect,
                          viewModel, false) &&
      newEffectId) {
    multipliers.emplace(*newEffectId, 1.0);
  }

  ImGui::PopID();
  ImGui::TreePop();
}

void DrawWaypoints(std::vector<std::pair<double, double>> &waypoints) {
  if (!ImGui::TreeNode("Waypoints")) {
    return;
  }

  int indexToRemove = -1;

  for (std::size_t index = 0; index < waypoints.size(); ++index) {
    ImGui::PushID(static_cast<int>(index));

    ImGui::Text("Waypoint %d", static_cast<int>(index + 1));
    ImGui::InputDouble("X", &waypoints[index].first);
    ImGui::InputDouble("Y", &waypoints[index].second);

    if (ImGui::SmallButton("Remove waypoint")) {
      indexToRemove = static_cast<int>(index);
    }

    ImGui::Separator();
    ImGui::PopID();
  }

  if (indexToRemove >= 0) {
    waypoints.erase(waypoints.begin() + indexToRemove);
  }

  if (ImGui::Button("Add waypoint")) {
    waypoints.emplace_back(0.0, 0.0);
  }

  ImGui::TreePop();
}
} // namespace inspector::widgets