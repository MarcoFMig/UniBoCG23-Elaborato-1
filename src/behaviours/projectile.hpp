#pragma once

#include <chrono>
#include "../object.hpp"
#include "../scene.hpp"

class Projectile : public Behaviour {
  private:
    std::shared_ptr<std::function<void()>> disableBGFunction;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    std::unique_ptr<Shader::UniformVariable> backgroundSetter;
    float deltaTime;
    float verticalSpeed;
  public:
    Projectile(std::shared_ptr<Object> objectReference,
      std::shared_ptr<Scene> sceneReference, float verticalSpeed);
    void setup() override;
    void frameCycle() override;
    void end() override;
};
