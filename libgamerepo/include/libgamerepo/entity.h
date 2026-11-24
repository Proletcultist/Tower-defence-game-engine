#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <optional>
#include <vector>

#include "bsoncxx/document/value.hpp"
#include "bsoncxx/document/view.hpp"

class GameRepository;

class EntityType {
  public:
    enum class Value : int32_t {
        Image = 0,
        PoisoningEffect = 1,
    };

    EntityType(Value value) : value(value) {}

    operator Value() const { return value; }
    static std::optional<EntityType> fromUnderlying(int32_t und) {
        switch (und) {
        case static_cast<int32_t>(Value::Image):
            return EntityType(Value::Image);
        case static_cast<int32_t>(Value::PoisoningEffect):
            return EntityType(Value::PoisoningEffect);
        default:
            return {};
        }
    }
    int32_t toUnderlying() const { return static_cast<int32_t>(value); }

  private:
    const Value value;
};

#include "libgamerepo/error.h"

class Entity {
    friend class GameRepository;

  private:
    const uint32_t id;
    const EntityType type;
    const std::string name;
    const std::vector<uint32_t> includers;
    const std::vector<uint32_t> backlinks;

  protected:
    Entity(uint32_t id, EntityType type, std::string name,
           std::vector<uint32_t> includers, std::vector<uint32_t> backlinks)
        : id(id), type(type), name(std::move(name)), includers(std::move(includers)),
          backlinks(std::move(backlinks)) {}
    static std::expected<Entity, EntityDeserializationError>
    fromBson(bsoncxx::document::view bson);
    virtual bsoncxx::document::value toBson() const;

  public:
    uint32_t getId() const { return id; }
    EntityType getType() const { return type; }
    const std::string &getName() const { return name; }
    const std::vector<uint32_t> &getIncluders() const { return includers; }
    const std::vector<uint32_t> &getBacklinks() const { return backlinks; }

    virtual ~Entity() = default;
};

class PoisoningEffect : Entity {
    friend class GameRepository;

  private:
    const uint32_t damage;
    const uint32_t duration; // msecs
    const uint32_t rate;     // msecs between damage

    static std::expected<PoisoningEffect, EntityDeserializationError>
    fromBson(bsoncxx::document::view bson);
    bsoncxx::document::value toBson() const override;

  public:
    PoisoningEffect(uint32_t id, std::string name,
                    std::vector<uint32_t> includers,
                    std::vector<uint32_t> backlinks, uint32_t damage,
                    uint32_t duration, uint32_t rate)
        : Entity(id, EntityType::Value::PoisoningEffect, name, includers,
                 backlinks),
          damage(damage), duration(duration), rate(rate) {}

    uint32_t getDamage() const { return damage; }
    uint32_t getDuration() const { return duration; }
    uint32_t getRate() const { return rate; }
};
