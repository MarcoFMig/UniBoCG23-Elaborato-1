#include <gl/glew.h>
#include <string>

#include "rawshader.hpp"

RawShader::RawShader(std::string name, std::string code, GLint type) {
  this->name = name;
  this->code = code;
  this->type = type;
}

const std::string& RawShader::getName() const {
  return this->name;
}

const std::string& RawShader::getCode() const {
  return this->code;
}

const GLint& RawShader::getType() const {
  return this->type;
}
