#pragma once

#include <exception>
#include <stdexcept>

class InvalidShaderException : public std::logic_error {
  public:
    InvalidShaderException();
};
