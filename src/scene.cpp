#include "scene.hpp"

#include <memory>
#include <functional>
#include <unordered_map>
#include <glm/mat4x4.hpp>

#include "object.hpp"
#include "subsystem.hpp"
#include "physicsmanager.hpp"
#include "shaderloader/shader.hpp"
#include "behaviours/ibehaviour.hpp"
#include "exceptions/invalidobjectexception.hpp"

Behaviour::Behaviour(std::shared_ptr<Object> objectReference, std::shared_ptr<Scene> sceneReference) {
  this->objectReference = objectReference;
  this->sceneReference = sceneReference;
}

Scene::Scene(std::shared_ptr<Shader> mainShader, glm::mat4 world) : world(world) {
  this->mainShader = mainShader;
  this->objectMapRef = std::make_shared<ObjectMap>(ObjectMap());
  this->subsystems.emplace(Subsystem::SubsystemType::physics,
    std::make_shared<PhysicsManager>(std::move(PhysicsManager(this->objectMapRef))));
}

Scene::Scene(std::shared_ptr<Shader> mainShader) : Scene(mainShader, glm::mat4()) {}

void Scene::setName(std::string newName) {
  this->sceneName = newName;
}

std::string Scene::getName() {
  return this->sceneName;
}

unsigned int Scene::addObject(Object object) {
  Object&& moveRef =  std::move(object);
  return this->addObject(moveRef);
}
unsigned int Scene::addObject(Object&& object) {
  return this->addObject(std::make_shared<Object>(std::move(object)));
}
unsigned int Scene::addObject(std::shared_ptr<Object> objectRef) {
  unsigned int providedId = idProvider.provideId();
  objectRef->setIdentifier(providedId);
  this->objectMapRef->emplace(providedId,
    std::make_pair<std::shared_ptr<Object>, BehaviourMap>(std::move(objectRef), BehaviourMap()));
  this->triggerSceneChange();
  return providedId;
}

void Scene::removeObject(unsigned int objectId) {
  std::list<unsigned int>::iterator foundObject =
     std::find(this->enqueuedRemoval.begin(),
      this->enqueuedRemoval.end(), objectId);
  if (foundObject == this->enqueuedRemoval.end()) {
    this->enqueuedRemoval.push_back(objectId);
  }
}
void Scene::removeObject(std::shared_ptr<Object> objectRef) {
  if (objectRef->getIdentifier().has_value()) {
    this->removeObject(objectRef->getIdentifier().value());
  } else {
    throw InvalidObjectException();
  }
}

unsigned int Scene::attachBehaviour(unsigned int objectId, std::shared_ptr<IBehaviour> behaviourRef) {
  if (this->objectMapRef->find(objectId) == this->objectMapRef->end()) {
    throw InvalidObjectException();
  }
  unsigned int providedId = this->idProvider.provideId();
  behaviourRef->setIdentifier(providedId);
  this->objectMapRef->at(objectId).second.emplace(providedId, std::move(behaviourRef));
  this->updateSubsystems(Subsystem::UpdateType::addition,
    this->objectMapRef->at(objectId).first,
    this->objectMapRef->at(objectId).second.at(providedId));
  this->objectMapRef->at(objectId).second.at(providedId)->setup();
  return providedId;
}

unsigned int Scene::attachBehaviour(std::shared_ptr<Object> object, std::shared_ptr<IBehaviour> behaviourRef) {
  if (object->getIdentifier().has_value()) {
    return this->attachBehaviour(object->getIdentifier().value(), behaviourRef);
  } else {
    throw InvalidObjectException();
  }
}

void Scene::detachBehaviour(unsigned int objectId, unsigned int behaviourId) {
  try {
    this->updateSubsystems(Subsystem::UpdateType::deletion,
      this->objectMapRef->at(objectId).first,
      this->objectMapRef->at(objectId).second.at(behaviourId));
    //this->objectMapRef->at(objectId).second.at(behaviourId).reset();
    //this->objectMapRef->at(objectId).second.erase(behaviourId);
  } catch (std::out_of_range error) {
    throw InvalidObjectException();
  }
}
void Scene::detachBehaviour(unsigned int objectId, std::shared_ptr<IBehaviour> behaviourRef) {
  try {
    if (behaviourRef->getIdentifier().has_value()) {
      this->detachBehaviour(objectId, behaviourRef->getIdentifier().value());
    } else {
      throw InvalidObjectException();
    }
  } catch (std::out_of_range error) {
    throw InvalidObjectException();
  }
}

void Scene::detachBehaviour(std::shared_ptr<Object> object, unsigned int behaviourId) {
  try {
    if (object->getIdentifier().has_value()) {
      this->detachBehaviour(object->getIdentifier().value(), behaviourId);
    } else {
      throw InvalidObjectException();
    }
  } catch (std::out_of_range error) {
    throw InvalidObjectException();
  }
}
void Scene::detachBehaviour(std::shared_ptr<Object> object, std::shared_ptr<IBehaviour> behaviourRef) {
  try {
    if (object->getIdentifier().has_value() && behaviourRef->getIdentifier().has_value()) {
      this->detachBehaviour(object->getIdentifier().value(), behaviourRef->getIdentifier().value());
    } else {
      throw InvalidObjectException();
    }
  } catch (std::out_of_range error) {
    throw InvalidObjectException();
  }
}

std::shared_ptr<Shader> Scene::getMainShader() {
  return this->mainShader;
}

void Scene::setText(std::list<std::shared_ptr<Label>> text) {
  this->text = text;
}
void Scene::addText(std::shared_ptr<Label> text) {
  this->text.push_back(text);
}
std::list<std::shared_ptr<Label>> Scene::getText() {
  return this->text;
}
std::list<std::shared_ptr<Label>>& Scene::peekText() {
  return this->text;
}
void Scene::clearText() {
  for (std::shared_ptr<Label> text : this->text) {
    text.reset();
  }
  this->text.clear();
}

void Scene::updateWindowData(
  float ratioedWindowHeight,
  float ratioedWindowWidth,
  int currentWidth,
  int currentHeight) {
  this->ratioedWindowHeight = ratioedWindowHeight;
  this->ratioedWindowWidth = ratioedWindowWidth;
  this->currentWidth = currentWidth;
  this->currentHeight = currentHeight;
}
float Scene::getRatioedWindowHeight() {
  return this->ratioedWindowHeight;
}
float Scene::getRatioedWindowWidth() {
  return this->ratioedWindowWidth;
}
int Scene::getCurrentWidth() {
  return this->currentWidth;
}
int Scene::getCurrentHeight() {
  return this->currentHeight;
}

void Scene::updateWorld(glm::mat4 world) {
  this->world = world;
  for (std::pair<Subsystem::SubsystemType, std::shared_ptr<Subsystem>> subsystem : this->subsystems) {
    subsystem.second->bindWorldSystem(world);
  }
}

unsigned int Scene::getElementCount() {
  return this->objectMapRef->size();
}

unsigned int Scene::getVertexCount() {
  unsigned int vertexCount = 0;
  for (std::pair<const unsigned int, ObjectAndBehavioursPair>& objectIdPair : *(this->objectMapRef)) {
    vertexCount += objectIdPair.second.first->getVertexCount();
  }
  return vertexCount;
}

unsigned int Scene::addSceneChangeListener(std::function<void()> listener) {
  unsigned int providedId = this->idProvider.provideId();
  this->registeredChangeListeners.emplace(providedId, listener);
  return providedId;
}

void Scene::removeSceneChangeListener(unsigned int listenerId) {
  this->idProvider.freeId(listenerId);
  this->registeredChangeListeners.erase(listenerId);
}

std::vector<std::shared_ptr<Mesh>> Scene::getMeshes() {
  std::vector<std::shared_ptr<Mesh>> meshList;
  meshList.reserve(this->objectMapRef->size());
  for (std::pair<const unsigned int, ObjectAndBehavioursPair>& objectIdPair : *(this->objectMapRef)) {
    meshList.push_back(objectIdPair.second.first);
  }
  return meshList;
}

void Scene::tick() {
  for (std::pair<const Subsystem::SubsystemType, std::shared_ptr<Subsystem>>& typeAndSubsystem : this->subsystems) {
    typeAndSubsystem.second->tick();
  }
  for (std::pair<const unsigned int, ObjectAndBehavioursPair>& objectIdPair : *(this->objectMapRef)) {
    for (std::pair<const unsigned int, std::shared_ptr<IBehaviour>>& idAndBehaviourPair : objectIdPair.second.second) {
      idAndBehaviourPair.second->frameCycle();
    }
  }
  for (unsigned int objToDelete : this->enqueuedRemoval) {
    try {
      for (std::pair<const unsigned int, std::shared_ptr<IBehaviour>>& singleBehaviour : this->objectMapRef->at(objToDelete).second) {
        singleBehaviour.second->end();
        this->detachBehaviour(objToDelete, singleBehaviour.second);
        this->idProvider.freeId(singleBehaviour.second->getIdentifier().value());
        singleBehaviour.second.reset();
      }
      this->objectMapRef->at(objToDelete).second.clear();
      this->objectMapRef->at(objToDelete).first.reset();
      this->objectMapRef->erase(objToDelete);
      this->triggerSceneChange();
    } catch (std::out_of_range error) {
      throw InvalidObjectException();
    }
  }
  this->enqueuedRemoval.clear();
}

void Scene::updateSubsystems(Subsystem::UpdateType reason,
                            std::shared_ptr<Object> targetObject,
                            std::shared_ptr<IBehaviour> responsible) {
  for (std::pair<const Subsystem::SubsystemType, std::shared_ptr<Subsystem>>& typeAndSubsystem : this->subsystems) {
    typeAndSubsystem.second->behaviourUpdate(reason, targetObject, responsible);
  }
}

Scene::BehaviourMap& Scene::retrieveBehaviourMap(unsigned int objectId) {
  return this->objectMapRef->at(objectId).second;
}

void Scene::triggerSceneChange() {
  for (std::pair<const unsigned int, std::function<void()>>& listener
    : this->registeredChangeListeners) {
    listener.second();
  }
}

void Scene::objectProbe(Object::EventType event, std::shared_ptr<Object> responsible) {
  switch (event) {
    case (Object::EventType::child_addition):
      if (!responsible->getIdentifier().has_value() ||
        this->objectMapRef->find(responsible->getIdentifier().value())
          == this->objectMapRef->end()) {
        unsigned int providedId = this->idProvider.provideId();
        this->objectMapRef->emplace(providedId, std::make_pair(responsible, BehaviourMap()));
      }
    break;
    default:
    return;
  }
}
