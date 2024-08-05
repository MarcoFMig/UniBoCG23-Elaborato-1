#pragma once
#include <exception>
#include <stdexcept>
#include <string>

class ShaderReadingException : public std::runtime_error {
  private:
    char errorMessage;
  public:
    ShaderReadingException();
    ShaderReadingException(const std::string& erroringShader);
};
