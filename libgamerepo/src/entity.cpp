#include <bit>
#include <expected>
#include <variant>

#include "bsoncxx/builder/basic/array.hpp"
#include "bsoncxx/builder/basic/document.hpp"
#include "bsoncxx/document/element.hpp"
#include "bsoncxx/document/value.hpp"
#include "bsoncxx/document/view.hpp"
#include "bsoncxx/types.hpp"
#include "bsoncxx/types/bson_value/view.hpp"
#include "libgamerepo/entity.h"

static std::expected<bsoncxx::types::bson_value::view,
                     EntityDeserializationError>
getBsonFieldValue(bsoncxx::document::view bson, std::string fieldname) {
    bsoncxx::document::element elem = bson[fieldname];
    if (!elem) {
        return std::unexpected(MissingField());
    } else {
        return elem.get_value();
    }
}

static std::expected<int32_t, EntityDeserializationError>
getBsonValueAsI32(bsoncxx::types::bson_value::view val) {
    if (val.type() != bsoncxx::type::k_int32) {
        return std::unexpected(WrongBsonValueType(bsoncxx::type::k_int32));
    } else {
        return val.get_int32();
    }
}

static std::expected<std::string, EntityDeserializationError>
getBsonValueAsString(bsoncxx::types::bson_value::view val) {
    if (val.type() != bsoncxx::type::k_string) {
        return std::unexpected(WrongBsonValueType(bsoncxx::type::k_string));
    } else {
        return std::string(val.get_string());
    }
}

static std::expected<bsoncxx::document::view, EntityDeserializationError>
getBsonValueAsBsonDocument(bsoncxx::types::bson_value::view val) {
    if (val.type() != bsoncxx::type::k_document) {
        return std::unexpected(WrongBsonValueType(bsoncxx::type::k_document));
    } else {
        return val.get_document();
    }
}

static std::expected<bsoncxx::array::view, EntityDeserializationError>
getBsonValueAsBsonArray(bsoncxx::types::bson_value::view val) {
    if (val.type() != bsoncxx::type::k_array) {
        return std::unexpected(WrongBsonValueType(bsoncxx::type::k_array));
    } else {
        return val.get_array();
    }
}

template <class T, class F> static auto foldBsonArrayToVector(F deserializer) {
    return [deserializer](bsoncxx::array::view arr)
               -> std::expected<std::vector<T>, EntityDeserializationError> {
        std::vector<T> ret;
        for (bsoncxx::array::element el : arr) {
            std::expected<T, EntityDeserializationError> expected_val = deserializer(el.get_value());
            if (!expected_val) {
                return std::unexpected(std::move(expected_val).error());
            }
            ret.push_back(*expected_val);
        }
        return ret;
    };
}

static auto addFieldNameContext(std::string name) {
    return
        [name](EntityDeserializationError err) -> EntityDeserializationError {
            return FieldDeserializationError(name, std::move(err));
        };
}

bsoncxx::document::value EntityMetadata::toBson() const {
    using bsoncxx::builder::basic::kvp;

    bsoncxx::types::b_int32 id{std::bit_cast<int32_t>(getId())};
    bsoncxx::types::b_int32 type{
        static_cast<int32_t>(getType().toUnderlying())};
    bsoncxx::types::b_string name{getName()};

    return bsoncxx::builder::basic::make_document(
        kvp("id", id), kvp("type", type), kvp("name", name));
}

bsoncxx::document::value EntityRelatioship::toBson() const {
    using bsoncxx::builder::basic::kvp;

    bsoncxx::types::b_int32 kind{
        static_cast<int32_t>(getKind().toUnderlying())};
    bsoncxx::types::b_string name{getName()};
    bsoncxx::types::b_int32 id{std::bit_cast<int32_t>(getId())};

    return bsoncxx::builder::basic::make_document(
        kvp("kind", kind), kvp("name", name), kvp("id", id));
}

bsoncxx::document::value Entity::toBson() const {
    using bsoncxx::builder::basic::kvp;

    bsoncxx::document::value metadata = getMetadata().toBson();

    bsoncxx::builder::basic::array relationships_builder;
    for (const EntityRelatioship &rel : getRelationships()) {
        relationships_builder.append(rel.toBson());
    }
    bsoncxx::array::value relationships = relationships_builder.extract();

    return bsoncxx::builder::basic::make_document(kvp("metadata", metadata), kvp("relationships", relationships));
}

bsoncxx::document::value PoisoningEffect::toBson() const {
    using bsoncxx::builder::basic::kvp;

    bsoncxx::document::value parent_fields = Entity::toBson();

    bsoncxx::types::b_int32 damage{std::bit_cast<int32_t>(getDamage())};
    bsoncxx::types::b_int32 duration{std::bit_cast<int32_t>(getDuration())};
    bsoncxx::types::b_int32 rate{std::bit_cast<int32_t>(getRate())};

    return bsoncxx::builder::basic::make_document(
        bsoncxx::builder::concatenate(parent_fields.view()),
        kvp("damage", damage), kvp("duration", duration), kvp("rate", rate));
}

std::expected<EntityMetadata, EntityDeserializationError>
EntityMetadata::fromBson(bsoncxx::document::view bson) {
    auto expected_id = getBsonFieldValue(bson, "id")
                        .and_then(getBsonValueAsI32)
                        .transform([](int32_t val){ return std::bit_cast<uint32_t>(val); })
                        .transform_error(addFieldNameContext("id"));
    if (!expected_id) {
        return std::unexpected(std::move(expected_id).error());
    }
    uint32_t id = *expected_id;

    auto expected_type = getBsonFieldValue(bson, "type")
                         .and_then(getBsonValueAsI32)
                         .transform(EntityType::fromUnderlying)
                         .and_then([](std::optional<EntityType> maybe_type) -> std::expected<EntityType, EntityDeserializationError>{
                            if (!maybe_type) {
                                return std::unexpected(WrongEntityTypeValue());
                            } else {
                                return *maybe_type;
                            }
                         })
                          .transform_error(addFieldNameContext("type"));
    if (!expected_type) {
        return std::unexpected(std::move(expected_type).error());
    }
    EntityType type = *expected_type;

    auto expected_name = getBsonFieldValue(bson, "name")
                            .and_then(getBsonValueAsString)
                            .transform_error(addFieldNameContext("name"));
    if (!expected_name) {
        return std::unexpected(std::move(expected_name).error());
    }
    std::string name = *expected_name;

    return EntityMetadata(id, type, std::move(name));
}

std::expected<EntityRelatioship, EntityDeserializationError>
EntityRelatioship::fromBson(bsoncxx::document::view bson) {
    auto expected_kind = getBsonFieldValue(bson, "kind")
                            .and_then(getBsonValueAsI32)
                            .transform(EntityRelationshipKind::fromUnderlying)
                            .and_then([](std::optional<EntityRelationshipKind> maybe_kind) -> std::expected<EntityRelationshipKind, EntityDeserializationError> {
                                if (!maybe_kind) {
                                    return std::unexpected(WrongEntityRelationKindValue());
                                } else {
                                    return *maybe_kind;
                                }
                            })
                          .transform_error(addFieldNameContext("kind"));
    if (!expected_kind) {
        return std::unexpected(std::move(expected_kind).error());
    }
    EntityRelationshipKind kind = *expected_kind;

    auto expected_name = getBsonFieldValue(bson, "name")
                            .and_then(getBsonValueAsString)
                            .transform_error(addFieldNameContext("name"));
    if (!expected_name) {
        return std::unexpected(std::move(expected_name).error());
    }
    std::string name = *expected_name;

    auto expected_id = getBsonFieldValue(bson, "id")
                        .and_then(getBsonValueAsI32)
                        .transform([](int32_t val){ return std::bit_cast<uint32_t>(val); })
                        .transform_error(addFieldNameContext("id"));
    if (!expected_id) {
        return std::unexpected(std::move(expected_id).error());
    }
    uint32_t id = *expected_id;

    return EntityRelatioship(kind, std::move(name), id);
}

std::expected<PoisoningEffect, EntityDeserializationError>
PoisoningEffect::fromBson(bsoncxx::document::view bson) {
    auto expected_metadata = getBsonFieldValue(bson, "metadata")
                            .and_then(getBsonValueAsBsonDocument)
                            .and_then(EntityMetadata::fromBson)
                            .transform_error(addFieldNameContext("metadata"));
    if (!expected_metadata) {
        return std::unexpected(std::move(expected_metadata).error());
    }
    EntityMetadata metadata = std::move(*expected_metadata);
    if (metadata.getType() != EntityType::Value::PoisoningEffect) {
        return std::unexpected(WrongEntityType(EntityType::Value::PoisoningEffect));
    }

    auto expected_relationships = getBsonFieldValue(bson, "relationships")
                                    .and_then(getBsonValueAsBsonArray)
                                    .and_then(foldBsonArrayToVector<EntityRelatioship>([](bsoncxx::types::bson_value::view val){
                                        return getBsonValueAsBsonDocument(val)
                                                .and_then(EntityRelatioship::fromBson);
                                    }))
                                    .transform_error(addFieldNameContext("relationships"));
    if (!expected_relationships) {
        return std::unexpected(std::move(expected_relationships).error());
    }
    std::vector<EntityRelatioship> relationships = *expected_relationships;

    auto expected_damage = getBsonFieldValue(bson, "damage")
                           .and_then(getBsonValueAsI32)
                           .transform([](int32_t val){ return std::bit_cast<uint32_t>(val); })
                           .transform_error(addFieldNameContext("damage"));
    if (!expected_damage) {
        return std::unexpected(std::move(expected_damage).error());
    }
    uint32_t damage = *expected_damage;

    auto expected_duration = getBsonFieldValue(bson, "duration")
                           .and_then(getBsonValueAsI32)
                           .transform([](int32_t val){ return std::bit_cast<uint32_t>(val); })
                           .transform_error(addFieldNameContext("duration"));
    if (!expected_duration) {
        return std::unexpected(std::move(expected_duration).error());
    }
    uint32_t duration = *expected_duration;

    auto expected_rate = getBsonFieldValue(bson, "rate")
                           .and_then(getBsonValueAsI32)
                           .transform([](int32_t val){ return std::bit_cast<uint32_t>(val); })
                           .transform_error(addFieldNameContext("rate"));
    if (!expected_rate) {
        return std::unexpected(std::move(expected_rate).error());
    }
    uint32_t rate = *expected_rate;

    return PoisoningEffect(
        metadata.getId(),
        std::move(metadata.getName()),
        std::move(relationships),
        damage,
        duration,
        rate
    );
}
