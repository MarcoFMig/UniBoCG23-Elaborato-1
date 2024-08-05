#include <exception>
#include <string>

#include "invalidshaderexception.hpp"

InvalidShaderException::InvalidShaderException()
  : std::logic_error("Selected shader does not exist!") {}
