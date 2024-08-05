#pragma once

#include <optional>

template <typename IdType>
class Identifiable {
  private:
    std::optional<IdType> identifier;
  public:
    virtual void setIdentifier(IdType identifier) {
      this->identifier = std::optional<IdType>(identifier);
    }
    virtual void setIdentifier(std::optional<IdType> identifier) {
      this->identifier = identifier;
    }
    virtual std::optional<IdType> getIdentifier() {
      return this->identifier;
    }
    virtual bool operator==(Identifiable<IdType>& otherIdentifiable) const {
      if (!otherIdentifiable.getIdentifier().has_value() || !this->identifier.has_value()) {
        return false;
      }
      return this->identifier.value() == otherIdentifiable.getIdentifier().value();
    }
    virtual ~Identifiable() = default;
};
