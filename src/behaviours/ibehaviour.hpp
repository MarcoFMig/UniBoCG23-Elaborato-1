#pragma once

#include <optional>

#include "../lightweightutils/identifiable.hpp"

/**
 * Represents a class that can control various aspects of an assigned object.
 */
class IBehaviour : public Identifiable<unsigned int> {
  public:
    /**
     * Called when the behaviour is attached to the object.
     */
    virtual void setup() {};
    /**
     * Called for every frame.
     */
    virtual void frameCycle() {};
    /**
     * Called when the behaviour is detached from the object.
     */
    virtual void end() {};
    /**
     * Destroys the behaviour.
     */
    virtual ~IBehaviour() = default;
};
