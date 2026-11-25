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

class EntityMetadata {
  private:
    const uint32_t id;
    const EntityType type;
    const std::string name;
  public:
    EntityMetadata(uint32_t id, EntityType type, std::string name) : id(id), type(type), name(std::move(name)) {}
    EntityMetadata(const EntityMetadata &other) : id(other.id), type(other.type), name(other.name) {}
    EntityMetadata(EntityMetadata &&other) : id(other.id), type(other.type), name(std::move(other.name)) {}

    static std::expected<EntityMetadata, EntityDeserializationError>
    fromBson(bsoncxx::document::view bson);
    bsoncxx::document::value toBson() const;

    uint32_t getId() const { return id; }
    EntityType getType() const { return type; }
    const std::string &getName() const { return name; }
};

class EntityRelationshipKind {
  public:
    enum class Value : int32_t {
        include = 0,
        reference = 1,

        part_of = 2,  // Reverse of include
        backlink = 3, // Reverse of reference
    };

    EntityRelationshipKind(Value value) : value(value) {}

    operator Value() const { return value; }
    static std::optional<EntityRelationshipKind> fromUnderlying(int32_t und) {
        switch (und) {
            case static_cast<int32_t>(Value::include):
                return EntityRelationshipKind(Value::include);
            case static_cast<int32_t>(Value::reference):
                return EntityRelationshipKind(Value::reference);
            case static_cast<int32_t>(Value::part_of):
                return EntityRelationshipKind(Value::part_of);
            case static_cast<int32_t>(Value::backlink):
                return EntityRelationshipKind(Value::backlink);
            default:
                return {};
        }
    }
    int32_t toUnderlying() const { return static_cast<int32_t>(value); }

  private:
    const Value value;
};

class EntityRelatioship {
    private:
        const EntityRelationshipKind kind;
        const std::string name;
        const uint32_t id;
    public:

        EntityRelatioship(EntityRelationshipKind kind, std::string name, uint32_t id) : kind(kind), name(std::move(name)), id(id) {}
        EntityRelatioship(const EntityRelatioship &other) : kind(other.kind), name(other.name), id(other.id) {}
        EntityRelatioship(EntityRelatioship &&other) : kind(other.kind), name(std::move(other.name)), id(other.id) {}

        static std::expected<EntityRelatioship, EntityDeserializationError>
        fromBson(bsoncxx::document::view bson);
        bsoncxx::document::value toBson() const;

        EntityRelationshipKind getKind() const { return kind; }
        const std::string &getName() const { return name; }
        uint32_t getId() const { return id; }
};

class EntityIndexEntry {
    friend class GameRepository;

  private:
    const EntityMetadata metadata;
    const std::vector<EntityRelatioship> relationships;

  protected:
    EntityIndexEntry(EntityMetadata metadata, std::vector<EntityRelatioship> relationships) : metadata(std::move(metadata)), relationships(std::move(relationships)) {}

  public:
    const EntityMetadata &getMetadata() const { return metadata; }
    const std::vector<EntityRelatioship> &getRelationships() const { return relationships; }
};

class Entity {
    friend class GameRepository;

  private:
    const EntityMetadata metadata;
    const std::vector<EntityRelatioship> relationships;

  protected:
    Entity(EntityMetadata metadata, std::vector<EntityRelatioship> relationships) : metadata(std::move(metadata)), relationships(std::move(relationships)) {}
    virtual bsoncxx::document::value toBson() const;

  public:
    const EntityMetadata &getMetadata() const { return metadata; }
    const std::vector<EntityRelatioship> &getRelationships() const { return relationships; }

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
    PoisoningEffect(uint32_t id, std::string name, std::vector<EntityRelatioship> relationships,
                    uint32_t damage, uint32_t duration, uint32_t rate)
        : Entity(EntityMetadata(id, EntityType::Value::PoisoningEffect, std::move(name)), std::move(relationships)),
          damage(damage), duration(duration), rate(rate) {}

    uint32_t getDamage() const { return damage; }
    uint32_t getDuration() const { return duration; }
    uint32_t getRate() const { return rate; }
};
