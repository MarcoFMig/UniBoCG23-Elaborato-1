#pragma once
#include <exception>
#include <stdexcept>
#include <string>

class InvalidFontException : public std::runtime_error {
  public:
    InvalidFontException();
    InvalidFontException(unsigned int fontIdentifier);
};
