#pragma once
#include <string>
#include <gl/glew.h>

class RawShader {
  protected:
    std::string name;
    std::string code;
    GLint type;
  public:
    RawShader(std::string name, std::string code, GLint type);
    const std::string& getName() const;
    const std::string& getCode() const;
    const GLint& getType() const;
};
