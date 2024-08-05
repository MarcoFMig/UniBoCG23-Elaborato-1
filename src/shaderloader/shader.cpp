#include "shader.hpp"

#include <string>
#include <functional>
#include <gl/glew.h>

Shader::Shader(unsigned int id, std::string name) {
  this->id = id;
  this->name = name;
}

Shader::UniformVariable Shader::queryUniform(std::string uniformName) {
  if (this->uniformVariableCache.find(uniformName) == this->uniformVariableCache.end()) {
    UniformVariable newVariable;
    newVariable.name = uniformName;
    newVariable.id = glGetUniformLocation(this->id, newVariable.name.c_str());
    this->uniformVariableCache[uniformName] = std::move(newVariable);
  }
  return this->uniformVariableCache[uniformName];
}

void Shader::editUniformVariable(std::shared_ptr<std::function<void()>> editingFunction) {
  (*editingFunction.get())();
}

unsigned int Shader::getId() {
  return this->id;
}

std::string Shader::getName() {
  return this->name;
}
