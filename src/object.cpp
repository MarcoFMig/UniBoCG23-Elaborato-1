#include "object.hpp"

#include <gl/glew.h>

#include "exceptions/invalidobjectexception.hpp"

Object::Object(
  int drawMode,
  unsigned int shaderProgramId,
  glm::vec3& position,
  glm::vec3& rotation,
  glm::vec3& scale,
  std::vector<glm::vec3>& vertexes,
  std::vector<glm::vec4>& colors,
  std::vector<unsigned int>& connections) 
  : Mesh(drawMode, shaderProgramId, position,
    rotation, scale, vertexes, colors, connections) {}

Object::Object(
  int drawMode,
  unsigned int shaderProgramId,
  std::vector<glm::vec3>& vertexes,
  std::vector<glm::vec4>& colors,
  std::vector<unsigned int>& connections)
  : Mesh(drawMode, shaderProgramId,
    vertexes, colors, connections) {}

Object::Object(
  int drawMode,
  unsigned int shaderProgramId,
  glm::vec3& position,
  glm::vec3& rotation,
  glm::vec3& scale,
  std::vector<glm::vec3>& vertexes,
  std::vector<glm::vec4>& colors)
  : Mesh(drawMode, shaderProgramId,
    vertexes, colors)  {}

Object::Object(
  int drawMode,
  unsigned int shaderProgramId,
  std::vector<glm::vec3>& vertexes,
  std::vector<glm::vec4>& colors)
  : Mesh(drawMode, shaderProgramId,
    vertexes, colors)  {}

Object::Object(
  unsigned int shaderProgramId,
  glm::vec3& position,
  glm::vec3& rotation,
  glm::vec3& scale,
  std::vector<glm::vec3>& vertexes,
  std::vector<glm::vec4>& colors,
  std::vector<unsigned int>& connections)
  : Mesh(GL_TRIANGLES, shaderProgramId, position,
    rotation, scale, vertexes, colors, connections) {}

Object::Object(
  unsigned int shaderProgramId,
  std::vector<glm::vec3>& vertexes,
  std::vector<glm::vec4>& colors,
  std::vector<unsigned int>& connections)
  : Mesh(GL_TRIANGLES, shaderProgramId,
    vertexes, colors, connections) {}

Object::Object(
  unsigned int shaderProgramId,
  glm::vec3& position,
  glm::vec3& rotation,
  glm::vec3& scale,
  std::vector<glm::vec3>& vertexes,
  std::vector<glm::vec4>& colors)
  : Mesh(GL_TRIANGLES, shaderProgramId,
    vertexes, colors)  {}

Object::Object(
  unsigned int shaderProgramId,
  std::vector<glm::vec3>& vertexes,
  std::vector<glm::vec4>& colors)
  : Mesh(GL_TRIANGLES, shaderProgramId,
    vertexes, colors)  {}

unsigned int Object::bindEventListener(std::function<void(Object::EventType, std::shared_ptr<Object>)>&& eventListener) {
  unsigned int generatedId = this->idProvider.provideId();
  this->listeners[generatedId] = std::move(eventListener);
  return generatedId;
}

void Object::unbindEventListener(unsigned int listenerId) {
  if (this->listeners.find(listenerId) != this->listeners.end()) {
    this->listeners.erase(listenerId);
    this->idProvider.freeId(listenerId);
  }
}

void Object::addChild(std::shared_ptr<Object> child) {
  if (!child->getIdentifier().has_value()) {
    throw InvalidObjectException();
  }
  if (child->getParent().has_value()) {
    child->getParent().value()->removeChild(child);
  }
  child->setParent(std::optional<std::shared_ptr<Object>>(this));
  this->children.emplace(child->getIdentifier().value(), child);
  this->triggerEvent(EventType::child_addition, child);
  // trigger event
}

void Object::removeChild(std::shared_ptr<Object> child) {
  if (!child->getIdentifier().has_value()) {
    throw InvalidObjectException();
  }
  this->triggerEvent(EventType::child_deletion, child);
  this->children.erase(child->getIdentifier().value());
  child->setParent(std::optional<std::shared_ptr<Object>>());
}

std::optional<std::shared_ptr<Object>> Object::getParent() {
  return this->parent;
}

std::unordered_map<unsigned int, std::shared_ptr<Object>> Object::getChildren() {
  return this->children;
}

void Object::triggerEvent(Object::EventType event, std::shared_ptr<Object> subject) {
  for (std::pair<unsigned int, std::function<void(Object::EventType, std::shared_ptr<Object>)>> idAndListener : this->listeners) {
    idAndListener.second(event, subject);
  }
}

void Object::setParent(std::optional<std::shared_ptr<Object>> parent) {
  this->parent = parent;
}

std::optional<unsigned int> Object::getIdentifier() {
   return this->identifier;
}
std::string Object::getName() {
  return this->name;
}
void Object::setName(std::string name) {
  this->name = name;
}
void Object::setIdentifier(std::optional<unsigned int> identifier) {
  this->identifier = identifier;
}
void Object::setIdentifier(unsigned int identifier) {
  this->identifier = std::move(std::optional<unsigned int>(identifier));
}
void Object::unsetIdentifier() {
  this->identifier = std::move(std::optional<unsigned int>());
}
