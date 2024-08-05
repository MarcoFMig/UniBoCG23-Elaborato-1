#pragma once

#include "mesh.hpp"

#include <string>

class Label : public Mesh {
  private:
    std::string text;
    glm::vec3 textColor;
  public:
    Label(std::string text);
    Label(std::string text, glm::vec3 textColor);
    std::string getText();

    void setText(std::string text);
    void setColor(glm::vec3 textColor);
    glm::vec3 getColor();
    glm::mat4 getTransformationMatrix() override;
};
