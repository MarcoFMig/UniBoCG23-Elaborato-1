#pragma once

#include <map>
#include <list>
#include <string>
#include <memory>

#include "shader.hpp"
#include "rawshader.hpp"

class ShaderManager {
  private:
    std::map<unsigned int, std::list<unsigned int>> shaderUniformMap;
    std::map<unsigned int, std::map<std::string, unsigned int>> idNameUniformMap;
    std::map<unsigned int, std::shared_ptr<Shader>> idShaderMap;
    std::map<std::string, std::shared_ptr<Shader>> nameShaderMap;
  public:
    ShaderManager() = default;
    void addShader(Shader&& shader);
    bool removeShader(unsigned int shaderId) noexcept;
    bool removeShader(std::string shaderName) noexcept;
    std::shared_ptr<Shader> readAndCompile(std::list<std::string> shaderFiles, std::string shaderName);
    std::shared_ptr<Shader> compileFromString(std::list<RawShader> shaderFiles, std::string shaderName);
};
