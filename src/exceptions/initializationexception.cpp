#include "initializationexception.hpp"

InitializationException::InitializationException(const char * message)
  : std::runtime_error(message) {}

InitializationException::InitializationException(std::string message)
  : std::runtime_error(message) {}