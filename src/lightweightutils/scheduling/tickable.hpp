#pragma once

/**
 * Represents a class that can be "ticked" in a scheduler.
 */
class Tickable {
  public:
    /**
     * Tick function.
     */
    virtual void tick() = 0;
    /**
     * Class destructor.
     */
    virtual ~Tickable() = default;
};
