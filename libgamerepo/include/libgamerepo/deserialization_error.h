#pragma once

#include <memory>
#include <variant>

#include "bsoncxx/types.hpp"
#include "libgamerepo/entity.h"

class MissingField;
class WrongBsonValueType;
class WrongEntityTypeValue;
class WrongEntityRelationKindValue;
class FieldDeserializationError;
class WrongEntityType;
typedef std::variant<MissingField, WrongBsonValueType, WrongEntityTypeValue,
                     FieldDeserializationError, WrongEntityType, WrongEntityRelationKindValue>
    EntityDeserializationError;

class MissingField {};

class WrongBsonValueType {
  private:
    const bsoncxx::type expected_type;

  public:
    WrongBsonValueType(bsoncxx::type expected_type)
        : expected_type(expected_type) {}

    const bsoncxx::type &getExpectedType() const { return expected_type; }
};

class WrongEntityTypeValue {};

class WrongEntityRelationKindValue {};

class WrongEntityType {
  private:
    const EntityType expected_type;

  public:
    WrongEntityType(EntityType expected_type)
        : expected_type(expected_type) {}

    const EntityType &getExpectedType() const { return expected_type; }
};

class FieldDeserializationError {
  private:
    const std::string name;
    std::unique_ptr<EntityDeserializationError> err;

  public:
    FieldDeserializationError(std::string name, EntityDeserializationError err)
        : name(name),
          err(std::make_unique<EntityDeserializationError>(std::move(err))) {}

    const std::string &getName() const { return name; }
    const EntityDeserializationError &getError() const { return *err; }
};
