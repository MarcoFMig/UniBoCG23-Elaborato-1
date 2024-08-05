#include "projectile.hpp"

#include <memory>
#include <chrono>
#include "../object.hpp"
#include "../scene.hpp"
#include "../logger/logger.hpp"

Projectile::Projectile(std::shared_ptr<Object> objectReference,
  std::shared_ptr<Scene> sceneReference, float verticalSpeed)
  : Behaviour(objectReference, sceneReference) {
  this->verticalSpeed = verticalSpeed;
  this->backgroundSetter = std::make_unique<Shader::UniformVariable>(
  this->sceneReference->getMainShader()->queryUniform("isBackground"));
  this->deltaTime = 0.0f;
  this->disableBGFunction = std::make_shared<std::function<void()>>([this]() {
      glUniform1i(this->backgroundSetter->id, (GLint) 0);
    }
  );
  this->objectReference->bindUniformUpdater(this->disableBGFunction);
  this->lastFrameTime = std::chrono::high_resolution_clock::now();
}

void Projectile::setup() {

}

void Projectile::frameCycle() {
  std::chrono::high_resolution_clock::time_point currentTime =
  std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsedTime =
    std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - this->lastFrameTime);
  this->deltaTime = elapsedTime.count();
  glm::vec3 newPosition = this->objectReference->getPosition();
    std::unique_ptr<Shader::UniformVariable> backgroundSetter;
  newPosition.y = newPosition.y + (this->verticalSpeed * deltaTime);
  this->objectReference->setPosition(newPosition);
  this->lastFrameTime = currentTime;
  if ((this->objectReference->getPosition().y > (this->sceneReference->getCurrentHeight() + 150.0f))
    || (this->objectReference->getPosition().y < 0)) {
    this->sceneReference->removeObject(this->objectReference);
  }
}

void Projectile::end() {
  //Logger::getInstance().log(Logger::fine, "I am die");
}
