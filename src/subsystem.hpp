#pragma once

#include <glm/mat4x4.hpp>

#include "lightweightutils/scheduling/tickable.hpp"
#include "behaviours/ibehaviour.hpp"
#include "object.hpp"

class Subsystem : public Tickable {
  public:
    enum UpdateType {
      addition,
      deletion,
      edit
    };
    enum SubsystemType {
      physics
    };
    virtual void behaviourUpdate(Subsystem::UpdateType reason,
      std::shared_ptr<Object> targetObject,
      std::shared_ptr<IBehaviour> responsible) {};
    virtual Subsystem::SubsystemType getSubsystemType() = 0;
    virtual void bindWorldSystem(glm::mat4 world) = 0;
};
