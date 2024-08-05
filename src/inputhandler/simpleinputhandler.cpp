#include "simpleinputhandler.hpp"

#include <functional>
#include <gl/freeglut.h>

#include "../lightweightutils/lwu.hpp"

SimpleInputHandler::SimpleInputHandler() {
  this->lastMouseX = 0;
  this->lastMouseY = 0;
  this->unboundObservers = generateUnboundMap();
}

void SimpleInputHandler::handleKeyDown(unsigned char key, int currentMouseX, int currentMouseY) {
  EventData event;
  event.key = key;
  event.mouseX = currentMouseX;
  event.mouseY = currentMouseY;
  event.type = KEYDOWN;
  this->inputVariationList.push_back(event);
  this->heldDownKeyList.insert(key);
}
void SimpleInputHandler::handleKeyUp(unsigned char key, int currentMouseX, int currentMouseY) {
  EventData event;
  event.key = key;
  event.mouseX = currentMouseX;
  event.mouseY = currentMouseY;
  event.type = KEYUP;
  this->inputVariationList.push_back(event);
  this->heldDownKeyList.erase(key);
}
void SimpleInputHandler::tick() {
  unsigned int targetIndex = this->inputVariationList.size();
  for (unsigned int index = 0; index < targetIndex; index ++) {
    EventData data = this->inputVariationList.front();
    if (this->boundObservers.find(data.key) != this->boundObservers.end()) {
      for (std::pair<unsigned int, ObserverStructure> idAndObserver : this->boundObservers[data.key][data.type]) {
        idAndObserver.second(data.key, data.mouseX, data.mouseY);
      }
    }
    for (std::pair<unsigned int, ObserverStructure> idAndUnboundObserver : this->unboundObservers[data.type]) {
      idAndUnboundObserver.second(data.key, data.mouseX, data.mouseY);
    }
    this->unboundObservers;
    this->inputVariationList.pop_front();
  }

  for (unsigned char key : this->heldDownKeyList) {
    for (std::pair<unsigned int, ObserverStructure> observer : this->boundObservers[key][KEYPRESS]) {
      observer.second(key, this->lastMouseX, this->lastMouseY);
    }
    for (std::pair<unsigned int, ObserverStructure> observer : this->unboundObservers[KEYPRESS]) {
      observer.second(key, this->lastMouseX, this->lastMouseY);
    }
  }
}

unsigned int SimpleInputHandler::registerKeyUpListener(unsigned char bindToKey, ObserverStructure observer) {
  unsigned int generatedIdentifier = this->idProvider.provideId();
  bool newEntry = false;
  if (this->boundObservers.find(bindToKey) == this->boundObservers.end()) {
    this->boundObservers[bindToKey] = this->generateUnboundMap();
  }
  this->boundObservers[bindToKey][KEYUP][generatedIdentifier] = std::move(observer);
  return generatedIdentifier;
}
unsigned int SimpleInputHandler::registerKeyUpListener(ObserverStructure observer) {
  unsigned int generatedIdentifier = this->idProvider.provideId();
  this->unboundObservers[KEYUP][generatedIdentifier] = std::move(observer);
  return generatedIdentifier;
}
unsigned int SimpleInputHandler::registerKeyDownListener(unsigned char bindToKey, ObserverStructure observer) {
  unsigned int generatedIdentifier = this->idProvider.provideId();
  bool newEntry = false;
  if (this->boundObservers.find(bindToKey) == this->boundObservers.end()) {
    this->boundObservers[bindToKey] = this->generateUnboundMap();
  }
  this->boundObservers[bindToKey][KEYDOWN][generatedIdentifier] = std::move(observer);
  return generatedIdentifier;
}
unsigned int SimpleInputHandler::registerKeyDownListener(ObserverStructure observer) {
  unsigned int generatedIdentifier = this->idProvider.provideId();
  this->unboundObservers[KEYDOWN][generatedIdentifier] = std::move(observer);
  return generatedIdentifier;
}
unsigned int SimpleInputHandler::registerKeyListener(unsigned char bindToKey, ObserverStructure observer) {
  unsigned int generatedIdentifier = this->idProvider.provideId();
  bool newEntry = false;
  if (this->boundObservers.find(bindToKey) == this->boundObservers.end()) {
    this->boundObservers[bindToKey] = this->generateUnboundMap();
  }
  this->boundObservers[bindToKey][KEYPRESS][generatedIdentifier] = std::move(observer);
  return generatedIdentifier;
}
unsigned int SimpleInputHandler::registerKeyListener(ObserverStructure observer) {
  unsigned int generatedIdentifier = this->idProvider.provideId();
  this->unboundObservers[KEYPRESS][generatedIdentifier] = std::move(observer);
  return generatedIdentifier;
}

void SimpleInputHandler::unregisterListener(unsigned int observerIdentifier) {
  this->observerReference[observerIdentifier].erase(observerIdentifier);
}

SimpleInputHandler::UnboundObserverMap SimpleInputHandler::generateUnboundMap() {
  UnboundObserverMap mapToReturn;
  for (unsigned int enumeration = 0; enumeration < KEYPRESS; enumeration ++) {
    mapToReturn[static_cast<Action>(enumeration)] = ObserverMap();
  }
  return std::move(mapToReturn);
}

// TODO: Finish this mess
void SimpleInputHandler::propagateUpdate(unsigned char key, int mouseX, int mouseY) {
  unsigned int targetIndex = this->inputVariationList.size();
  for (unsigned int index = 0; index < targetIndex; index ++) {
    EventData data = this->inputVariationList.front();
    if (this->boundObservers.find(data.key) != this->boundObservers.end()) {
      for (std::pair<unsigned int, ObserverStructure> idAndObserver : this->boundObservers[data.key][data.type]) {
        idAndObserver.second(data.key, data.mouseX, data.mouseY);
      }
    }
    for (std::pair<unsigned int, ObserverStructure> idAndUnboundObserver : this->unboundObservers[data.type]) {
      idAndUnboundObserver.second(data.key, data.mouseX, data.mouseY);
    }
    this->unboundObservers;
    this->inputVariationList.pop_front();
  }
}
