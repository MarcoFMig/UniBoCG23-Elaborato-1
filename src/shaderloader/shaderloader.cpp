#include <string>
#include <vector>
#include <algorithm>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "exceptions/shadernotinitializedexception.hpp"
#include "exceptions/shadercreationexception.hpp"
#include "exceptions/shaderreadingexception.hpp"
#include "shaderloader.hpp"
#include "shader.hpp"

void ShaderLoader::addShader(std::list<RawShader> shader) {
  for (RawShader singleShader : shader) {
    this->shadersToLoad.push_back(singleShader);
  }
}
void ShaderLoader::addShader(RawShader shader) {
  this->shadersToLoad.push_back(shader);
}

GLuint ShaderLoader::getProgram() {
  return this->shaderProgram;
}

void ShaderLoader::readShadersFromFile(std::list<std::string> shaderPath) {
  for (std::string singlePath : shaderPath) {
    readShadersFromFile(singlePath);
  }
}
void ShaderLoader::readShadersFromFile(std::string shaderPath) {
  try {
    std::ifstream shaderFile;
    std::string readShader = "";
    GLint shaderType;
    shaderFile.open(shaderPath);
    if (shaderFile.is_open()) {
      std::string shaderExtension = shaderPath.substr(shaderPath.find_last_of(".") + 1);
      std::string shaderName = shaderPath.substr(0, shaderPath.find_last_of("."));
      if (std::find(this->allowedFragmentShaderExtensions.begin(),
        this->allowedFragmentShaderExtensions.end(), shaderExtension) != allowedFragmentShaderExtensions.end()) {
        shaderType = GL_FRAGMENT_SHADER;
      } else if (std::find(this->allowedVertexShaderExtensions.begin(),
        this->allowedVertexShaderExtensions.end(), shaderExtension) != allowedVertexShaderExtensions.end()) {
        shaderType = GL_VERTEX_SHADER;
      } else {
        throw ShaderReadingException("Unrecognized shader type: " + shaderPath);
      }
      char readChar;
      while(shaderFile.get(readChar)) {
        readShader += readChar;
      }
      shaderFile.close();
      RawShader shader(shaderName, readShader, shaderType);
      this->shadersToLoad.push_back(shader);
    } else {
      throw ShaderReadingException("Unable to open shader file: " + shaderPath);
    }
  } catch (std::ifstream::failure& exception) {
    throw ShaderReadingException("Failure while reading shader file: " + shaderPath + "\nDumping exception:\n" + exception.what());
  } catch (std::out_of_range& exception) {
    throw ShaderReadingException(shaderPath);
  }
}

void ShaderLoader::readShadersFromFile(const char * fileName) {
  readShadersFromFile(std::string(fileName));
}

void ShaderLoader::readShadersFromFile(const char ** fileArray, const unsigned int size) {
  std::list<std::string> shaderPaths;
  for (unsigned int index = 0; index < size; index++) {
    shaderPaths.push_back(std::string(fileArray[index]));
  }
  readShadersFromFile(shaderPaths);
}

void ShaderLoader::compileShaders(const char* compiledShaderName) {
  std::string shaderName = compiledShaderName;
  this->compileShaders(shaderName);
}

void ShaderLoader::compileShaders(std::string compiledShaderName) {
  GLint infoParam;
  GLuint shaderProgram = glCreateProgram();

  if (shaderProgram == 0) {
      throw ShaderCreationException();
  }

  for (RawShader shader : this->shadersToLoad) {
    GLuint shaderObject = glCreateShader(shader.getType());
    
    if (shaderObject == 0) {
      throw ShaderCreationException();
    }

    const char* shaderCodeChars = shader.getCode().c_str();
    const GLint length = shader.getCode().length();
    glShaderSource(shaderObject, 1, &shaderCodeChars, &length);
    glCompileShader(shaderObject);

    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &infoParam);

    if (!infoParam) {
      GLint infoLogLength;
      std::string fullInfoLog;
      glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
      fullInfoLog.resize(infoLogLength);
      glGetShaderInfoLog(shaderObject, infoLogLength, NULL, fullInfoLog.data());
      throw ShaderCreationException(fullInfoLog);
    }
    glAttachShader(shaderProgram, shaderObject);
    this->loadedShaders.push_back(shader);
  }
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &infoParam);

  if (infoParam == 0) {
    GLint infoLogLength;
    std::string fullInfoLog;
    glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
    fullInfoLog.resize(infoLogLength);
    glGetShaderInfoLog(shaderProgram, infoLogLength, NULL, fullInfoLog.data());
    throw ShaderCreationException(fullInfoLog);
  }
  //glUseProgram(shaderProgram);
  this->shaderProgram = shaderProgram;
  this->compiledShader = std::optional<Shader>(std::move(Shader(this->getProgram(), compiledShaderName)));
}

Shader ShaderLoader::getShader() {
  if (this->compiledShader.has_value()) {
    return this->compiledShader.value();
  } else {
    throw ShaderNotInitializedException();
  }
}

const std::list<RawShader>& ShaderLoader::getShadersToLoad() const {
  return this->shadersToLoad;
}
const std::list<RawShader>& ShaderLoader::getLoadedShaders() const {
  return this->loadedShaders;
}
