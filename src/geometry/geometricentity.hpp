#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class GeometricEntity {
  protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
  public:
    GeometricEntity();
    GeometricEntity(glm::vec3& position, glm::vec3& degrees, glm::vec3& scale);
    virtual void setPosition(glm::vec3 position);
    virtual glm::vec3& getPosition();
    virtual void setRotation(glm::vec3 degrees);
    virtual glm::vec3& getRotation();
    virtual void setScale(glm::vec3 scale);
    virtual glm::vec3& getScale();
    virtual ~GeometricEntity() = default;
};
