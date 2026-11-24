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

static std::expected<uint32_t, EntityDeserializationError>
getBsonValueAsU32(bsoncxx::types::bson_value::view val) {
    if (val.type() != bsoncxx::type::k_int32) {
        return std::unexpected(WrongBsonValueType(bsoncxx::type::k_int32));
    } else {
        return std::bit_cast<uint32_t>(static_cast<int32_t>(val.get_int32()));
    }
}

static std::expected<EntityType, EntityDeserializationError>
getBsonValueAsEntityType(bsoncxx::types::bson_value::view val) {
    if (val.type() != bsoncxx::type::k_int32) {
        return std::unexpected(WrongBsonValueType(bsoncxx::type::k_int32));
    } else {
        auto maybe_type = EntityType::fromUnderlying(val.get_int32());
        if (maybe_type) {
            return maybe_type.value();
        } else {
            return std::unexpected(WrongEntityTypeValue());
        }
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
            auto expected_val = deserializer(el.get_value());
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

bsoncxx::document::value Entity::toBson() const {
    using bsoncxx::builder::basic::kvp;

    bsoncxx::types::b_int32 id{std::bit_cast<int32_t>(getId())};
    bsoncxx::types::b_int32 type{
        static_cast<int32_t>(getType().toUnderlying())};
    bsoncxx::types::b_string name{getName()};

    bsoncxx::builder::basic::array includers_builder;
    for (uint32_t i : getIncluders()) {
        bsoncxx::types::b_int32 includer_id{std::bit_cast<int32_t>(i)};
        includers_builder.append(includer_id);
    }
    bsoncxx::array::value includers = includers_builder.extract();

    bsoncxx::builder::basic::array backlinks_builder;
    for (uint32_t i : getBacklinks()) {
        bsoncxx::types::b_int32 backlink_id{std::bit_cast<int32_t>(i)};
        backlinks_builder.append(backlink_id);
    }
    bsoncxx::array::value backlinks = backlinks_builder.extract();

    return bsoncxx::builder::basic::make_document(
        kvp("id", id), kvp("type", type), kvp("name", name),
        kvp("includers", includers), kvp("backlinks", backlinks));
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

std::expected<Entity, EntityDeserializationError>
Entity::fromBson(bsoncxx::document::view bson) {
    auto expected_id = getBsonFieldValue(bson, "id")
                           .and_then(getBsonValueAsU32)
                           .transform_error(addFieldNameContext("id"));
    if (!expected_id) {
        return std::unexpected(std::move(expected_id).error());
    }
    uint32_t id = *expected_id;

    auto expected_type = getBsonFieldValue(bson, "type")
                             .and_then(getBsonValueAsEntityType)
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

    auto expected_includers =
        getBsonFieldValue(bson, "includers")
            .and_then(getBsonValueAsBsonArray)
            .and_then(foldBsonArrayToVector<uint32_t>(getBsonValueAsU32))
            .transform_error(addFieldNameContext("name"));
    if (!expected_includers) {
        return std::unexpected(std::move(expected_includers).error());
    }
    std::vector<uint32_t> includers = *expected_includers;

    auto expected_backlinks =
        getBsonFieldValue(bson, "backlinks")
            .and_then(getBsonValueAsBsonArray)
            .and_then(foldBsonArrayToVector<uint32_t>(getBsonValueAsU32))
            .transform_error(addFieldNameContext("backlinks"));
    if (!expected_backlinks) {
        return std::unexpected(std::move(expected_backlinks).error());
    }
    std::vector<uint32_t> backlinks = *expected_backlinks;

    return Entity(id, type, name, includers, backlinks);
}

std::expected<PoisoningEffect, EntityDeserializationError>
PoisoningEffect::fromBson(bsoncxx::document::view bson) {
    auto expected_base = Entity::fromBson(bson);
    if (!expected_base) {
        return std::unexpected(std::move(expected_base).error());
    }
    Entity base = *expected_base;

    if (base.getType() != EntityType::Value::PoisoningEffect) {
        return std::unexpected(WrongEntityType(EntityType::Value::PoisoningEffect));
    }

    auto expected_damage = getBsonFieldValue(bson, "damage")
                           .and_then(getBsonValueAsU32)
                           .transform_error(addFieldNameContext("damage"));
    if (!expected_damage) {
        return std::unexpected(std::move(expected_damage).error());
    }
    uint32_t damage = *expected_damage;

    auto expected_duration = getBsonFieldValue(bson, "duration")
                           .and_then(getBsonValueAsU32)
                           .transform_error(addFieldNameContext("duration"));
    if (!expected_duration) {
        return std::unexpected(std::move(expected_duration).error());
    }
    uint32_t duration = *expected_duration;

    auto expected_rate = getBsonFieldValue(bson, "rate")
                           .and_then(getBsonValueAsU32)
                           .transform_error(addFieldNameContext("rate"));
    if (!expected_rate) {
        return std::unexpected(std::move(expected_rate).error());
    }
    uint32_t rate = *expected_rate;

    return PoisoningEffect(
        base.getId(),
        std::move(base.getName()),
        std::move(base.getIncluders()),
        std::move(base.getBacklinks()),
        damage,
        duration,
        rate
    );
}
