#include "invalidobjectexception.hpp"

InvalidObjectException::InvalidObjectException()
  : std::runtime_error("An error occourred while accessing a non existing object Id!") {}

InvalidObjectException::InvalidObjectException(unsigned int objectIdentifier)
  : std::runtime_error("An error occourred while accessing object Id: " + objectIdentifier) {}
