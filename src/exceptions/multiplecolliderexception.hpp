#pragma once
#include <exception>
#include <stdexcept>
#include <string>

class MultipleColliderException : public std::runtime_error {
  public:
    MultipleColliderException();
    MultipleColliderException(unsigned int objectIdentifier);
};
