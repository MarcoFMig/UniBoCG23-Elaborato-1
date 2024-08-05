#pragma once

#include <map>
#include <memory>
#include <string>
#include <functional>

class Shader {
  public:
    struct UniformVariable {
      std::string name;
      unsigned int id;
    };
  private:
    unsigned int id;
    std::string name;
    std::map<std::string, UniformVariable> uniformVariableCache;
  public:
    Shader(unsigned int id, std::string name);
    UniformVariable queryUniform(std::string uniformName);
    void editUniformVariable(std::shared_ptr<std::function<void()>> editingFunction);
    unsigned int getId();
    std::string getName();
};
