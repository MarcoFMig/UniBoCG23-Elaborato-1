#include "charactercontroller.hpp"

#include <cmath>
#include <chrono>
#include <memory>
#include <iostream>
#include <functional>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "../shaderloader/shader.hpp"
#include "../simplemath/simplemath.hpp"
#include "physics/rectangularcollider.hpp"
#include "../simplemath/hermitecurve.hpp"
#include "projectile.hpp"
#include "../object.hpp"
#include "../scene.hpp"

CharacterController::CharacterController(std::shared_ptr<Object> objectReference,
  std::shared_ptr<Scene> sceneReference)
  : Behaviour(objectReference, sceneReference),
    inputHandler(SimpleInputHandler::getInstance()) {
  this->lastFrameTime = std::chrono::high_resolution_clock::now();
  this->rightAxisDownFunction = [this](unsigned int key, int mouseX, int mouseY) {
    this->rightDown = true;
  };
  this->rightAxisUpFunction = [this](unsigned int key, int mouseX, int mouseY) {
    this->rightDown = false;
  };
  this->leftAxisDownFunction = [this](unsigned int key, int mouseX, int mouseY) {
    this->leftDown = true;
  };
  this->leftAxisUpFunction = [this](unsigned int key, int mouseX, int mouseY) {
    this->leftDown = false;
  };
  this->leftDown = false;
  this->rightDown = false;
  this->decayRate = 2.0f;
  this->deltaTime = 0.0f;
  this->maxSpeed = 5.0f;
  this->acceleration = 0.5f;
  this->horizontalAxis = 0.0f;
  this->axisIncrease = 2.0f;
  this->debugText = std::make_shared<Label>("placeholder", glm::vec3(1.0f, 1.0f, 1.0f));
  this->debugText->getPosition().x = (this->sceneReference->getCurrentWidth() / 2.0f) - 200.0f;
  this->debugText->getPosition().y = 10.0f;
  this->debugText->getPosition().z = 2.0f;
  this->debugText->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
  this->debugText->setVisible(true);
}

void CharacterController::setup() {
  this->inputHandler.registerKeyDownListener('a', this->leftAxisDownFunction);
  this->inputHandler.registerKeyDownListener('A', this->leftAxisDownFunction);
  this->inputHandler.registerKeyDownListener('d', this->rightAxisDownFunction);
  this->inputHandler.registerKeyDownListener('D', this->rightAxisDownFunction);
  this->inputHandler.registerKeyUpListener('a', this->leftAxisUpFunction);
  this->inputHandler.registerKeyUpListener('A', this->leftAxisUpFunction);
  this->inputHandler.registerKeyUpListener('d', this->rightAxisUpFunction);
  this->inputHandler.registerKeyUpListener('D', this->rightAxisUpFunction);
  this->sceneReference->addText(this->debugText);
}

void CharacterController::frameCycle() {
  std::chrono::high_resolution_clock::time_point currentTime =
    std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsedTime =
    std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - this->lastFrameTime);
  this->deltaTime = elapsedTime.count();
  if (!this->rightDown && !this->leftDown) {
    if (this->horizontalAxis != 0.0f) {
      float axisDelta = decayRate * this->deltaTime;
      if (std::abs(axisDelta) > std::abs(this->horizontalAxis)) {
        this->horizontalAxis = 0.0f;
      } else {
        this->horizontalAxis -= std::copysign(axisDelta, this->horizontalAxis);
      }
    }
  } else if (!(this->rightDown && this->leftDown)) {
    if (this->rightDown) {
      this->horizontalAxis += axisIncrease * this->deltaTime;
    }
    if (this->leftDown) {
      this->horizontalAxis -= axisIncrease * this->deltaTime;
    }
  }
  this->lastFrameTime = currentTime;
  this->horizontalAxis = std::clamp(this->horizontalAxis, __LOWER_AXIS_BOUND, __UPPER_AXIS_BOUND);
  this->objectReference->getPosition().x +=
    SimpleMath::lerp(-this->maxSpeed, this->maxSpeed, (this->horizontalAxis + 1.0f) / 2.0f);
  this->debugText->setText(std::string("X axis: ") + std::to_string(this->horizontalAxis));
}

void CharacterController::end() {
  
}
