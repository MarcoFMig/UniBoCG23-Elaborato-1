#pragma once

#include <vector>
#include <glm/vec3.hpp>

#include "../scene.hpp"
#include "../object.hpp"
#include "../shaderloader/shader.hpp"

class Enemy : public Behaviour {
  private:

  public:
    Enemy(std::shared_ptr<Object> objectReference,
      std::shared_ptr<Scene> sceneReference);
};
