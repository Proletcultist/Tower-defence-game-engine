#pragma once

#include <map>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace entity
{
using EntityId = std::size_t;

struct LevelProperties
{
};

struct GameMapBackgroundProperties
{
    double cellSize = 0.0;
    unsigned int height = 0;
    unsigned int width = 0;

    std::vector<std::vector<EntityId>> tiles;
};

struct GameMapProperties
{
    std::optional<EntityId> background;
    std::vector<EntityId> spots;
    std::vector<EntityId> paths;
};

struct TextureProperties
{
    std::string imagePath;
};

struct EnemyTypeProperties
{
    std::optional<EntityId> texture;
    double speed = 0.0;
    unsigned int hitPoints = 0;
    unsigned int reward = 0;
    std::map<EntityId, double> effectMultipliers;
};

struct EnemyWaveProperties
{
    std::vector<EntityId> paths;
    unsigned int duration = 0;
    std::optional<EntityId> nextWave;
};

struct EnemyWaveOnPathProperties
{
    std::optional<EntityId> path;
    std::vector<EntityId> enemyTypes;
    unsigned int spawnRate = 0;
};

struct EnemyPathProperties
{
    std::vector<std::pair<double, double>> waypoints;
};

struct TowerTypeProperties
{
    std::optional<EntityId> texture;
    std::optional<EntityId> damageType;
    double attackRange = 0.0;
    int cooldown = 0;
    unsigned int sellPrice = 0;
    std::vector<std::pair<unsigned int, EntityId>> promotions;
};

struct TowerSpotProperties
{
    double x = 0.0;
    double y = 0.0;
    std::optional<EntityId> texture;
    std::vector<std::pair<unsigned int, EntityId>> buildableTowers;
};

enum class DamageKind
{
    Projectile,
    InstantReach,
    HomingProjectile
};

struct DamageTypeProperties
{
    std::optional<EntityId> texture;
    std::vector<EntityId> effects;
    DamageKind kind = DamageKind::Projectile;
};


struct HealingAbilityProperties
{
    unsigned int hitPoints = 0;
};

struct DamageAbilityProperties
{
    std::optional<EntityId> damageType;
};

struct AbilityProperties
{
    std::variant<HealingAbilityProperties, DamageAbilityProperties> ability =
        HealingAbilityProperties{};
};

struct PoisoningEffectProperties
{
    unsigned int damage = 0;
    unsigned int poisoningDuration = 0;
    double poisoningRate = 0.0;
};

struct DamageEffectProperties
{
    unsigned int damage = 0;
};

struct SlownessEffectProperties
{
    double multiplier = 1.0;
    unsigned int duration = 0;
};

using EffectProperties = std::variant<
    PoisoningEffectProperties,
    DamageEffectProperties,
    SlownessEffectProperties
>;

using EntityProperties = std::variant<
    std::monostate,
    LevelProperties,
    GameMapBackgroundProperties,
    GameMapProperties,
    TextureProperties,
    EnemyTypeProperties,
    EnemyWaveProperties,
    EnemyWaveOnPathProperties,
    EnemyPathProperties,
    TowerTypeProperties,
    TowerSpotProperties,
    DamageTypeProperties,
    AbilityProperties,
    EffectProperties
>;
}