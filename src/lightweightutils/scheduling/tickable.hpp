#pragma once

class Tickable {
  public:
    virtual void tick() = 0;
    virtual ~Tickable() = default;
};
