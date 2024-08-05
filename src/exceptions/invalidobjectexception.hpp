#pragma once
#include <exception>
#include <stdexcept>
#include <string>

class InvalidObjectException : public std::runtime_error {
  public:
    InvalidObjectException();
    InvalidObjectException(unsigned int objectIdentifier);
};
