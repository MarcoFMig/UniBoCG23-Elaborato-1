#include <exception>
#include <string>

#include "shadercreationexception.hpp"

ShaderCreationException::ShaderCreationException()
  : std::runtime_error("An error occourred during shader loading phase!") {}

ShaderCreationException::ShaderCreationException(std::string errorMessage)
  : std::runtime_error("An error occourred during shader loading phase!\n" + errorMessage) {}
