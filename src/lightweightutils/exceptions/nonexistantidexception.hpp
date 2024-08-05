#pragma once

#include <stdexcept>

class NonExistantIDException : public std::logic_error {
  public:
    NonExistantIDException(unsigned int invalidIdentifier, const char* operationName);
};
