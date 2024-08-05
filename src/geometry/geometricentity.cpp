#include "geometricentity.hpp"

#include <glm/vec3.hpp>

GeometricEntity::GeometricEntity() {
  this->position = glm::vec3(0.0f, 0.0f, 0.0f);
  this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}
GeometricEntity::GeometricEntity(glm::vec3& position, glm::vec3& degrees,
                                glm::vec3& scale) {
  this->position = position;
  this->rotation = degrees;
  this->scale = scale;
}

void GeometricEntity::setPosition(glm::vec3 position) {
  this->position = position;
}
glm::vec3& GeometricEntity::getPosition() {
  return this->position;
}
void GeometricEntity::setRotation(glm::vec3 degrees) {
  this->rotation = degrees;
}
glm::vec3& GeometricEntity::getRotation() {
  return this->rotation;
}
void GeometricEntity::setScale(glm::vec3 scale) {
  this->scale = scale;
}
glm::vec3& GeometricEntity::getScale() {
  return this->scale;
}
