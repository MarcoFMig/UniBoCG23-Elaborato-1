#pragma once
#include <exception>
#include <stdexcept>
#include <string>

class ShaderCreationException : public std::runtime_error {
  public:
    ShaderCreationException();
    ShaderCreationException(std::string errorMessage);
};
