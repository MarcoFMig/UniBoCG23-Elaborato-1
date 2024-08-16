#pragma once

#include <memory>
#include <utility>
#include <unordered_map>
#include <glm/mat4x4.hpp>

#include "behaviours/physics/rectangularcollider.hpp"
#include "behaviours/ibehaviour.hpp"
#include "subsystem.hpp"
#include "object.hpp"

class PhysicsManager : public Subsystem {
  private:
    struct ActiveCollider {
      bool ticked;
    };
    glm::mat4 world;
    std::shared_ptr<std::unordered_map<unsigned int, std::pair<std::shared_ptr<Object>,
      std::unordered_map<unsigned int, std::shared_ptr<IBehaviour>>>>> objectMapRef;
    std::unordered_map<unsigned int, std::pair<std::shared_ptr<Object>, std::shared_ptr<RectangularCollider>>> trackedRectColliders;
    //std::unordered_map<unsigned int, ActiveCollider> activeCollisions;
    inline bool isInsideBoxCollider(
      std::pair<const unsigned int, std::pair<std::shared_ptr<Object>,
        std::shared_ptr<RectangularCollider>>>& firstCollider,
      std::pair<const unsigned int, std::pair<std::shared_ptr<Object>,
        std::shared_ptr<RectangularCollider>>>& secondCollider);
    inline void triggerCorrespondingListeners(std::pair<const unsigned int,
      std::pair<std::shared_ptr<Object>,
      std::shared_ptr<RectangularCollider>>>& colliderA,
      std::pair<const unsigned int,
      std::pair<std::shared_ptr<Object>,
      std::shared_ptr<RectangularCollider>>>& colliderB);
  public:
    PhysicsManager() = delete;
    /**
     * Constructs a PhysicsManager instance.
     * @param objectMapRef    reference to the object map of the scene
     */
    PhysicsManager(std::shared_ptr<std::unordered_map<unsigned int, std::pair<std::shared_ptr<Object>,
      std::unordered_map<unsigned int, std::shared_ptr<IBehaviour>>>>> objectMapRef);
    virtual void behaviourUpdate(Subsystem::UpdateType reason,
      std::shared_ptr<Object> targetObject,
      std::shared_ptr<IBehaviour> responsible) override;
    PhysicsManager::SubsystemType getSubsystemType() override;
    void bindWorldSystem(glm::mat4 world) override;
    void tick() override;
};
