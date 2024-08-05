#pragma once

#include <optional>

#include "../lightweightutils/identifiable.hpp"

class IBehaviour : public Identifiable<unsigned int> {
  public:
    virtual void setup() {};
    virtual void frameCycle() {};
    virtual void end() {};
    virtual ~IBehaviour() = default;
};
