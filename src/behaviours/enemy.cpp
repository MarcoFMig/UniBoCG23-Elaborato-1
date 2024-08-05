#include "enemy.hpp"

#include "../scene.hpp"
#include "../object.hpp"
#include "../shaderloader/shader.hpp"

Enemy::Enemy(std::shared_ptr<Object> objectReference,
  std::shared_ptr<Scene> sceneReference)
    : Behaviour(objectReference, sceneReference) {
  
}
