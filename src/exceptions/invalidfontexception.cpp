#include "invalidfontexception.hpp"

InvalidFontException::InvalidFontException()
  : std::runtime_error("An error occourred while accessing a non existing font Id!") {}

InvalidFontException::InvalidFontException(unsigned int fontIdentifier)
  : std::runtime_error("An error occourred while accessing font Id: " + fontIdentifier) {}
