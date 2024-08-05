#pragma once

#include <memory>
#include <chrono>
#include <gl/glew.h>
#include "data/firstgamedata.hpp"
#include "../inputhandler/simpleinputhandler.hpp"
#include "../scene.hpp"
#include "../object.hpp"

#define __LOWER_AXIS_BOUND -1.0f
#define __UPPER_AXIS_BOUND 1.0f

class CharacterController : public Behaviour {
  private:
    struct EntityPrototype {
      std::vector<glm::vec3> geometry;
      std::vector<glm::vec4> colors;
    };
    SimpleInputHandler& inputHandler;
    EntityPrototype projectilePrototype;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    std::function<void(unsigned int, int, int)> rightAxisUpFunction;
    std::function<void(unsigned int, int, int)> rightAxisDownFunction;
    std::function<void(unsigned int, int, int)> leftAxisUpFunction;
    std::function<void(unsigned int, int, int)> leftAxisDownFunction;
    std::shared_ptr<Label> debugText;
    bool leftDown;
    bool rightDown;
    float decayRate;
    float deltaTime;
    float horizontalAxis;
    float axisIncrease;
    float maxSpeed;
    float acceleration;

  public:
    CharacterController(std::shared_ptr<Object> objectReference,
      std::shared_ptr<Scene> sceneReference);
    void setup() override;
    void frameCycle() override;
    void end() override;
};
