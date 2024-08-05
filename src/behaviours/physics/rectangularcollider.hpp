#pragma once

#include <memory>
#include <functional>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "../../object.hpp"
#include "../ibehaviour.hpp"
#include "../../lightweightutils/idprovider.hpp"

class RectangularCollider : public IBehaviour {
  private:
    glm::vec4 pointA;
    glm::vec4 pointB;

    glm::vec3 pointAWorldCache;
    glm::vec3 pointBWorldCache;

    IDProvider idProvider;

    std::unordered_map<unsigned int, std::shared_ptr<std::function<void(std::shared_ptr<Object>, std::shared_ptr<Object>)>>> collisionListeners;
    std::unordered_map<unsigned int, std::shared_ptr<std::function<void(std::shared_ptr<Object>, std::shared_ptr<Object>)>>> collisionExitListeners;
    std::unordered_map<unsigned int, std::shared_ptr<std::function<void(std::shared_ptr<Object>, std::shared_ptr<Object>)>>> collisionEnterListeners;
  public:
    RectangularCollider(glm::vec4 topRightCorner, glm::vec4 bottomLeftCorner);
    RectangularCollider(glm::vec3 topRightCorner, glm::vec3 bottomLeftCorner);
    virtual ~RectangularCollider() = default;
    virtual void setWorldTopRight(glm::vec3 pointAWorld);
    virtual void setWorldBottomLeft(glm::vec3 pointBWorld);
    virtual glm::vec3 getWorldTopRight();
    virtual glm::vec3 getWorldBottomLeft();
    virtual unsigned int registerCollisionEnterListener(std::shared_ptr
      <std::function<
        void(std::shared_ptr<Object>, std::shared_ptr<Object>)>> collisionEnterListener);
    virtual unsigned int registerCollisionExitListener(std::shared_ptr
      <std::function<
        void(std::shared_ptr<Object>, std::shared_ptr<Object>)>> collisionEnterListener);
    virtual unsigned int registerCollisionListener(std::shared_ptr
      <std::function<
        void(std::shared_ptr<Object>, std::shared_ptr<Object>)>> collisionEnterListener);
    virtual void unRegisterCollisionEnterListener(unsigned int identifier);
    virtual void unRegisterCollisionExitListener(unsigned int identifier);
    virtual void unRegisterCollisionListener(unsigned int identifier);
    virtual void triggerCollisionEnter(std::shared_ptr<Object> objectA, std::shared_ptr<Object> objectB);
    virtual void triggerCollisionExit(std::shared_ptr<Object> objectA, std::shared_ptr<Object> objectB);
    virtual void triggerCollision(std::shared_ptr<Object> objectA, std::shared_ptr<Object> objectB);
    virtual glm::vec4 getTopRight();
    virtual glm::vec4 getBottomLeft();
};
