#pragma once

#include <glm/mat4x4.hpp>

#include "lightweightutils/scheduling/tickable.hpp"
#include "behaviours/ibehaviour.hpp"
#include "object.hpp"

/**
 * A class representing a subsystem, that handles various special predefined behaviours.
 */
class Subsystem : public Tickable {
  public:
    /**
     * Types of events that might cause a behaviour update.
     */
    enum UpdateType {
      /**
       * When an object has a new behaviour.
       */
      addition,
      /**
       * When a behaviour is removed from an object.
       */
      deletion,
      /**
       * @deprecated
       */
      edit
    };
    /**
     * Types of subsystems that can exist.
     */
    enum SubsystemType {
      /**
       * A physics subsystem.
       */
      physics
    };
    /**
     * Tells the subsystem to update itself due to a change of a behaviour in an object.
     * @param reason        cause of the update
     * @param targetObject  object cause of the update
     * @param responsible   behaviour responsible for this update
     */
    virtual void behaviourUpdate(Subsystem::UpdateType reason,
      std::shared_ptr<Object> targetObject,
      std::shared_ptr<IBehaviour> responsible) {};
    /**
     * Returns the subsystem's type
     */
    virtual Subsystem::SubsystemType getSubsystemType() = 0;
    /**
     * Updates the subsystem's world matrix
     * @param world         world matrix
     */
    virtual void bindWorldSystem(glm::mat4 world) = 0;
};
