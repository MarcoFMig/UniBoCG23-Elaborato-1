#pragma once
#include <exception>
#include <stdexcept>
#include <string>

class InitializationException : public std::runtime_error {
  public:
    InitializationException(const char * message);
    InitializationException(std::string message);
};
