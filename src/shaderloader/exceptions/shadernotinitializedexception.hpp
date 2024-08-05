#pragma once

#include <exception>
#include <stdexcept>

class ShaderNotInitializedException : public std::runtime_error {
  public:
    ShaderNotInitializedException();
};
