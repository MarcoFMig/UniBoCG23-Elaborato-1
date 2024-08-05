#include "physicsmanager.hpp"

#include <iostream>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

#include "initdata.hpp"
#include "logger/logger.hpp"
#include "exceptions/invalidobjectexception.hpp"
#include "exceptions/multiplecolliderexception.hpp"

PhysicsManager::PhysicsManager(std::shared_ptr<std::unordered_map<unsigned int, std::pair<std::shared_ptr<Object>,
      std::unordered_map<unsigned int, std::shared_ptr<IBehaviour>>>>> objectMapRef) {
  this->objectMapRef = objectMapRef;
}

PhysicsManager::SubsystemType PhysicsManager::getSubsystemType() {
  return PhysicsManager::SubsystemType::physics;
}

void PhysicsManager::bindWorldSystem(glm::mat4 world) {
  this->world = world;
}

void PhysicsManager::behaviourUpdate(Subsystem::UpdateType reason,
  std::shared_ptr<Object> targetObject,
  std::shared_ptr<IBehaviour> responsible) {
  switch (reason) {
    case (Subsystem::UpdateType::addition):
      if (auto castResponsible = std::dynamic_pointer_cast<RectangularCollider>(responsible)) {
        if (this->trackedRectColliders.find(targetObject->getIdentifier().value()) != this->trackedRectColliders.end()) {
          throw MultipleColliderException(targetObject->getIdentifier().value());
        }
        this->trackedRectColliders.emplace(targetObject->getIdentifier().value(),
          std::pair<std::shared_ptr<Object>, std::shared_ptr<RectangularCollider>>(
            targetObject, castResponsible));
      }
    break;
    case (Subsystem::UpdateType::deletion):
      if (auto castResponsible = std::dynamic_pointer_cast<RectangularCollider>(responsible)) {
        try {
          //this->trackedRectColliders.at(targetObject->getIdentifier().value()).first.reset();
          //this->trackedRectColliders.at(targetObject->getIdentifier().value()).second.reset();
          this->trackedRectColliders.erase(targetObject->getIdentifier().value());
        } catch (std::out_of_range error) {
          throw InvalidObjectException();
        }
      }
    break;
    default:
    break;
  }
}

void PhysicsManager::tick() {
  std::list<unsigned int> visited;
  for (std::pair<const unsigned int,
    std::pair<std::shared_ptr<Object>,
    std::shared_ptr<RectangularCollider>>>& firstCollider : this->trackedRectColliders) {
    visited.push_back(firstCollider.first);
    for (std::pair<const unsigned int,
      std::pair<std::shared_ptr<Object>,
      std::shared_ptr<RectangularCollider>>>& secondCollider : this->trackedRectColliders) {
      if (std::find(visited.begin(), visited.end(), secondCollider.first) != visited.end()) {
        continue;
      }
      if (isInsideBoxCollider(firstCollider, secondCollider)) {
        //printf("%s -- %s\n", firstCollider.second.first->getName().c_str(), secondCollider.second.first->getName().c_str());
        //triggerCorrespondingListeners(firstCollider, secondCollider);
        firstCollider.second.second->triggerCollision(firstCollider.second.first, secondCollider.second.first);
        secondCollider.second.second->triggerCollision(secondCollider.second.first, firstCollider.second.first);
      }
    }
  }
}

bool PhysicsManager::isInsideBoxCollider(
  std::pair<const unsigned int, std::pair<std::shared_ptr<Object>,
    std::shared_ptr<RectangularCollider>>>& firstCollider,
  std::pair<const unsigned int, std::pair<std::shared_ptr<Object>,
    std::shared_ptr<RectangularCollider>>>& secondCollider) {
  // NOTE: Very inefficient mess.
  glm::mat4 firstTransformationMatrix = firstCollider.second.first->getTransformationMatrix();
  glm::mat4 secondTransformationMatrix = secondCollider.second.first->getTransformationMatrix();
  glm::vec4 firstPointA = firstTransformationMatrix * firstCollider.second.second->getTopRight();
  glm::vec4 firstPointB = firstTransformationMatrix * firstCollider.second.second->getBottomLeft();
  glm::vec4 secondPointA = secondTransformationMatrix * secondCollider.second.second->getTopRight();
  glm::vec4 secondPointB = secondTransformationMatrix * secondCollider.second.second->getBottomLeft();
  firstCollider.second.second->setWorldTopRight(firstPointA);
  firstCollider.second.second->setWorldBottomLeft(firstPointB);
  secondCollider.second.second->setWorldTopRight(secondPointA);
  secondCollider.second.second->setWorldBottomLeft(secondPointB);
  if ((((firstPointA.x <= secondPointA.x) && (firstPointA.x >= secondPointB.x))
    && ((firstPointA.y <= secondPointA.x) && (firstPointA.y >= secondPointB.y)))
    || (((firstPointB.x >= secondPointB.x) && (firstPointB.x <= secondPointA.x))
    && ((firstPointB.y >= secondPointB.x) && (firstPointB.y <= secondPointA.y)))) {
    return true;
  }
  return false;
}

void PhysicsManager::triggerCorrespondingListeners(std::pair<const unsigned int,
  std::pair<std::shared_ptr<Object>,
  std::shared_ptr<RectangularCollider>>>& colliderA,
  std::pair<const unsigned int,
  std::pair<std::shared_ptr<Object>,
  std::shared_ptr<RectangularCollider>>>& colliderB) {
  // NOTE: Saved for now, might implement in the future
  /*if (this->activeCollisions.find(colliderA.first) == this->inCollisionColliders.end()) {
    ActiveCollider colliderStatusA;
    colliderStatusA.ticked = false;
    this->activeCollisions.emplace(colliderStatusA);
    ActiveCollider colliderStatusB;
    colliderStatusB.ticked = false;
    
  }*/
}
