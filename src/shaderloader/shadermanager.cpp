#include "shadermanager.hpp"

#include <gl/glew.h>

#include "exceptions/invalidshaderexception.hpp"
#include "shaderloader.hpp"
#include "rawshader.hpp"
#include "shader.hpp"

void ShaderManager::addShader(Shader&& shader) {
  
}

bool ShaderManager::removeShader(unsigned int shaderId) noexcept {
  if (this->idShaderMap.find(shaderId) == this->idShaderMap.end()) {
    return false;
  } else {
    glDeleteShader(shaderId);
    return true;
  }
}

bool ShaderManager::removeShader(std::string shaderName) noexcept {
  if (this->nameShaderMap.find(shaderName) == this->nameShaderMap.end()) {
    return false;
  } else {
    glDeleteShader(this->nameShaderMap[shaderName].get()->getId());
    return true;
  }
}

std::shared_ptr<Shader> ShaderManager::compileFromString(std::list<RawShader> shaderFiles, std::string shaderName) {
  ShaderLoader loader;
  for (RawShader shaderFile : shaderFiles) {
    loader.addShader(shaderFile);
  }
  loader.compileShaders(shaderName);
  std::shared_ptr<Shader> shaderPtr = std::make_shared<Shader>(loader.getShader());
  this->nameShaderMap[shaderName] = shaderPtr;
  this->idShaderMap[shaderPtr.get()->getId()] = shaderPtr;
  return shaderPtr;
}

std::shared_ptr<Shader> ShaderManager::readAndCompile(std::list<std::string> shaderFiles, std::string shaderName) {
  ShaderLoader loader;
  for (std::string shaderFile : shaderFiles) {
    loader.readShadersFromFile(shaderFile);
  }
  loader.compileShaders(shaderName);
  std::shared_ptr<Shader> shaderPtr = std::make_shared<Shader>(loader.getShader());
  this->nameShaderMap[shaderName] = shaderPtr;
  this->idShaderMap[shaderPtr.get()->getId()] = shaderPtr;
  return shaderPtr;
}
