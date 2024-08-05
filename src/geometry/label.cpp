#include "label.hpp"

#include <string>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.hpp"

Label::Label(std::string text) : Label(text, glm::vec3(0.5f, 0.0f, 0.0f)) {}

Label::Label(std::string text, glm::vec3 textColor)
 : Mesh() {
  this->text = text;
  this->textColor = textColor;
}

std::string Label::getText() {
  return this->text;
}

void Label::setText(std::string text) {
  this->text = text;
}

void Label::setColor(glm::vec3 color) {
  this->textColor = color;
}

glm::vec3 Label::getColor() {
  return this->textColor;
}

glm::mat4 Label::getTransformationMatrix() {
	this->transformationMatrix = glm::mat4(1.0);
  this->transformationMatrix = glm::translate(this->transformationMatrix, this->position);
  this->transformationMatrix = glm::rotate(this->transformationMatrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  this->transformationMatrix = glm::scale(this->transformationMatrix, this->scale);
  return this->transformationMatrix;
}
