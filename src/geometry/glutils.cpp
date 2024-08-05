#include "glutils.hpp"

#include <gl/glew.h>

unsigned int glutils::queryUniformVariable(unsigned int shaderProgram, const char* name) {
  return glGetUniformLocation(shaderProgram, name);
}
unsigned int glutils::queryUniformVariable(unsigned int shaderProgram, std::string name) {
  return glGetUniformLocation(shaderProgram, name.c_str());
}
