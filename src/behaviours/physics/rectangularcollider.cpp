#include "rectangularcollider.hpp"

#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "../../logger/logger.hpp"
#include "../../lightweightutils/exceptions/nonexistantidexception.hpp"

RectangularCollider::RectangularCollider(glm::vec4 topRightCorner, glm::vec4 bottomLeftCorner) {
  this->pointA = topRightCorner;
  this->pointB = bottomLeftCorner;
}
RectangularCollider::RectangularCollider(glm::vec3 topRightCorner, glm::vec3 bottomLeftCorner) {
  this->pointA = glm::vec4(topRightCorner, 1.0f);
  this->pointB = glm::vec4(bottomLeftCorner, 1.0f);
}

unsigned int RectangularCollider::registerCollisionEnterListener(std::shared_ptr
  <std::function<
    void(std::shared_ptr<Object>, std::shared_ptr<Object>)>> collisionEnterListener) {
  unsigned int providedId = this->idProvider.provideId();
  this->collisionEnterListeners.emplace(providedId, collisionEnterListener);
  return providedId;
}
unsigned int RectangularCollider::registerCollisionExitListener(std::shared_ptr
  <std::function<
    void(std::shared_ptr<Object>, std::shared_ptr<Object>)>> collisionEnterListener) {
  unsigned int providedId = this->idProvider.provideId();
  this->collisionExitListeners.emplace(providedId, collisionEnterListener);
  return providedId;
}
unsigned int RectangularCollider::registerCollisionListener(std::shared_ptr
  <std::function<
    void(std::shared_ptr<Object>, std::shared_ptr<Object>)>> collisionEnterListener) {
  unsigned int providedId = this->idProvider.provideId();
  this->collisionListeners.emplace(providedId, collisionEnterListener);
  return providedId;
}

void RectangularCollider::unRegisterCollisionEnterListener(unsigned int identifier) {
  this->idProvider.freeId(identifier);
  this->collisionEnterListeners.erase(identifier);
}
void RectangularCollider::unRegisterCollisionExitListener(unsigned int identifier) {
  this->idProvider.freeId(identifier);
  this->collisionExitListeners.erase(identifier);
}
void RectangularCollider::unRegisterCollisionListener(unsigned int identifier) {
  this->idProvider.freeId(identifier);
  this->collisionListeners.erase(identifier);
}

void RectangularCollider::triggerCollision(std::shared_ptr<Object> objectA, std::shared_ptr<Object> objectB) {
  for (std::pair<unsigned int, std::shared_ptr<std::function<void(std::shared_ptr<Object>, std::shared_ptr<Object>)>>> idAndListener : this->collisionListeners) {
    (*idAndListener.second)(objectA, objectB);
  }
}

void RectangularCollider::triggerCollisionEnter(std::shared_ptr<Object> objectA, std::shared_ptr<Object> objectB) {
  for (std::pair<unsigned int, std::shared_ptr<std::function<void(std::shared_ptr<Object>, std::shared_ptr<Object>)>>> idAndListener : this->collisionListeners) {
    (*idAndListener.second)(objectA, objectB);
  }
}

void RectangularCollider::triggerCollisionExit(std::shared_ptr<Object> objectA, std::shared_ptr<Object> objectB) {
  for (std::pair<unsigned int, std::shared_ptr<std::function<void(std::shared_ptr<Object>, std::shared_ptr<Object>)>>> idAndListener : this->collisionListeners) {
    (*idAndListener.second)(objectA, objectB);
  }
}

glm::vec4 RectangularCollider::getTopRight() {
  return this->pointA;
}
glm::vec4 RectangularCollider::getBottomLeft() {
  return this->pointB;
}

void RectangularCollider::setWorldTopRight(glm::vec3 pointAWorld) {
  this->pointAWorldCache = pointAWorld; 
}
void RectangularCollider::setWorldBottomLeft(glm::vec3 pointBWorld) {
  this->pointBWorldCache = pointBWorld;
}

glm::vec3 RectangularCollider::getWorldTopRight() {
  return this->pointAWorldCache;
}
glm::vec3 RectangularCollider::getWorldBottomLeft() {
  return this->pointBWorldCache;
}
