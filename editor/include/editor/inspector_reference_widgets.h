#pragma once

#include "editor/hierarchy_view_model.h"

#include <map>
#include <optional>
#include <utility>
#include <vector>

namespace inspector::widgets {
void DrawUnsigned(const char *label, unsigned int &value);

bool DrawEntityReference(const char *label,
                         std::optional<entity::EntityId> &selectedId,
                         entity::EntityType targetType,
                         const hierarchy::HierarchyViewModel &viewModel,
                         bool allowNone = true);

void DrawEntityReferenceList(const char *label,
                             std::vector<entity::EntityId> &ids,
                             entity::EntityType targetType,
                             const hierarchy::HierarchyViewModel &viewModel);

void DrawEntityReferencePairs(
    const char *label,
    std::vector<std::pair<unsigned int, entity::EntityId>> &values,
    const char *numberLabel, entity::EntityType targetType,
    const hierarchy::HierarchyViewModel &viewModel);

void DrawEffectMultipliers(std::map<entity::EntityId, double> &multipliers,
                           const hierarchy::HierarchyViewModel &viewModel);

void DrawWaypoints(std::vector<std::pair<double, double>> &waypoints);
} // namespace inspector::widgets