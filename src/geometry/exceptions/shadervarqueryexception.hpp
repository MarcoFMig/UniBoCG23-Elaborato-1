#pragma once
#include <exception>
#include <stdexcept>
#include <string>

class ShaderVarQueryException : public std::runtime_error {
  public:
    ShaderVarQueryException();
    ShaderVarQueryException(const std::string& erroringShaderVar);
};
