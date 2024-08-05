#include "shadervarqueryexception.hpp"

ShaderVarQueryException::ShaderVarQueryException()
  : std::runtime_error("An error occourred while accessing a shader uniform variable!") {}

ShaderVarQueryException::ShaderVarQueryException(const std::string& variableName)
  : std::runtime_error("An error occourred while accessing a shader uniform variable: " + variableName) {}
