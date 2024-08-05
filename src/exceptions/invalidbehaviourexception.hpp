#pragma once
#include <exception>
#include <stdexcept>
#include <string>

class InvalidBehaviourException : public std::runtime_error {
  public:
    InvalidBehaviourException();
    InvalidBehaviourException(unsigned int objectIdentifier);
};
