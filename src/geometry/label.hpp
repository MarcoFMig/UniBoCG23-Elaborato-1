#pragma once

#include "mesh.hpp"

#include <string>

/**
 * Class that represents a label containing text.
 */
class Label : public Mesh {
  private:
    std::string text;
    glm::vec3 textColor;
  public:
    /**
     * Constructs a label given text.
     * @param text      text for the label
     */
    Label(std::string text);
    /**
     * Constructs a label given text and a color.
     * @param text      text for the label
     * @param textColor text color for the label
     */
    Label(std::string text, glm::vec3 textColor);
    /**
     * @returns         text inside the label
     */
    std::string getText();

    /**
     * Updates the text inside the label.
     * @param text      new text for the label
     */
    void setText(std::string text);
    /**
     * Updates the text's color.
     * @param textColor new color for the label
     */
    void setColor(glm::vec3 textColor);
    /**
     * @returns         the label's current color
     */
    glm::vec3 getColor();
    /**
     * @returns         the label's transformation matrix
     */
    glm::mat4 getTransformationMatrix() override;
};
