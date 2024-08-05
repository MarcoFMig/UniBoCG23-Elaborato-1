#include "nonexistantidexception.hpp"

#include <cstring>
#include <stdexcept>

// FIXME: Add identifier in error output
NonExistantIDException
  ::NonExistantIDException(unsigned int invalidIdentifier, const char* operationName) 
  : std::logic_error(
    std::string("Invalid identifier")) {}
