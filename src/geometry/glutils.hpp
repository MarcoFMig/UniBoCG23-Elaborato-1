#pragma once

#include <string>

namespace glutils {
  unsigned int queryUniformVariable(unsigned int shaderProgram, const char* name);
  unsigned int queryUniformVariable(unsigned int shaderProgram, std::string name);
  
};
