#pragma once

namespace menu {
enum class Theme { Dark, Light };

void ApplyTheme(Theme theme);

bool DrawMenuBar(Theme &currentTheme);
} // namespace menu