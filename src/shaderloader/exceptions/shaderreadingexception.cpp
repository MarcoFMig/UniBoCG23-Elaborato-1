#include <exception>
#include <string>

#include "shaderreadingexception.hpp"

ShaderReadingException::ShaderReadingException()
  : std::runtime_error("An error occurred while reading a shader!") {}

ShaderReadingException::ShaderReadingException(const std::string& erroringShader)
  : std::runtime_error("Error occurred while reading shader: " + erroringShader) {}
