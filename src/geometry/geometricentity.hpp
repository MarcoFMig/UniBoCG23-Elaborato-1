#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

/**
 * A class that represents a simple geometric entity.
 */
class GeometricEntity {
  protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
  public:
    /**
     * Constructs an empty geometric entity.
     */
    GeometricEntity();
    /**
     * Constructs a geometric entity.
     * @param position  starting position
     * @param degrees   starting rotation
     * @param scale     starting scale
     */
    GeometricEntity(glm::vec3& position, glm::vec3& degrees, glm::vec3& scale);
    /**
     * Updates the object's position.
     * @param postiion  the object's new position
     */
    virtual void setPosition(glm::vec3 position);
    /**
     * @returns the object's position
     */
    virtual glm::vec3& getPosition();
    /**
     * Updates the object's rotation.
     * @param degrees   the object's new rotation
     */
    virtual void setRotation(glm::vec3 degrees);
    /**
     * @returns the object's rotation
     */
    virtual glm::vec3& getRotation();
    /**
     * Updates the object's scale
     * @param scale     the object's new scale
     */
    virtual void setScale(glm::vec3 scale);
    /**
     * @returns the object's scale
     */
    virtual glm::vec3& getScale();
    /**
     * Destroys the geometric entity.
     */
    virtual ~GeometricEntity() = default;
};
