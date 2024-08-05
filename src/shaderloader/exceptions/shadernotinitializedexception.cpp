#include <exception>
#include <string>

#include "shadernotinitializedexception.hpp"

ShaderNotInitializedException::ShaderNotInitializedException()
  : std::runtime_error("Attempted to edit values for an uninitialized shader program!") {}
