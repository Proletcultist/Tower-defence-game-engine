#pragma once

#include "editor/entity_properties.h"
#include "editor/hierarchy_view_model.h"

namespace inspector::map_properties {
void DrawGameMapBackgroundProperties(
    entity::GameMapBackgroundProperties &properties,
    const hierarchy::HierarchyViewModel &viewModel);

void DrawGameMapProperties(entity::GameMapProperties &properties,
                           const hierarchy::HierarchyViewModel &viewModel);

void DrawTextureProperties(entity::TextureProperties &properties);

void DrawEnemyPathProperties(entity::EnemyPathProperties &properties);
} // namespace inspector::map_properties