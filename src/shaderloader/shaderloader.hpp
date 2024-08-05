#pragma once
#include <filesystem>
#include <optional>
#include <gl/glew.h>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <list>

#include "rawshader.hpp"
#include "shader.hpp"

class ShaderLoader {
  private:
    GLuint shaderProgram;
    std::optional<Shader> compiledShader;
    std::list<RawShader> shadersToLoad;
    std::list<RawShader> loadedShaders;
    const std::array<const char*, 2> allowedFragmentShaderExtensions = {"fs", "frag"};
    const std::array<const char*, 2> allowedVertexShaderExtensions = {"vs", "vert"};
  public:
    ShaderLoader() = default;
    void readShadersFromFile(const char *fileName);
    void readShadersFromFile(std::string fileName);
    void readShadersFromFile(const char **fileArray, const unsigned int size);
    void readShadersFromFile(std::list<std::string> fileList);
    void addShader(std::list<RawShader> shader);
    void addShader(RawShader shader);
    GLuint getProgram();
    Shader getShader();
    const std::list<RawShader>& getShadersToLoad() const;
    const std::list<RawShader>& getLoadedShaders() const;
    void compileShaders(const char* compiledShaderName);
    void compileShaders(std::string compiledShaderName);
};
