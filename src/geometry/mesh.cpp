#include "mesh.hpp"

#include <memory>
#include <unordered_map>
#include <gl/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glutils.hpp"

Mesh::Mesh(
          int drawMode,
          unsigned int shaderProgramId,
          glm::vec3& position,
          glm::vec3& rotation,
          glm::vec3& scale,
          std::vector<glm::vec3>& vertexes,
          std::vector<glm::vec4>& colors,
          std::vector<unsigned int>& connections)
          : GeometricEntity(position, rotation, scale) {
  this->geometryChanged = true;
  this->uniformValuesUpdated = true;
  this->transformationCacheValid = false;
  this->visible = true;
  this->shaderProgramId = shaderProgramId;
  this->modelUniformId = glutils::queryUniformVariable(this->shaderProgramId, "Model");
  this->drawMode = drawMode;
  this->vertexes = std::move(vertexes);
  this->colors = std::move(colors);
  this->connections = std::optional(std::move(connections));
}
Mesh::Mesh(
          int drawMode,
          unsigned int shaderProgramId,
          std::vector<glm::vec3>& vertexes,
          std::vector<glm::vec4>& colors,
          std::vector<unsigned int>& connections)
          : GeometricEntity() {
  this->geometryChanged = true;
  this->uniformValuesUpdated = true;
  this->transformationCacheValid = false;
  this->visible = true;
  this->shaderProgramId = shaderProgramId;
  this->modelUniformId = glutils::queryUniformVariable(this->shaderProgramId, "Model");
  this->drawMode = drawMode;
  this->vertexes = std::move(vertexes);
  this->colors = std::move(colors);
  this->connections = std::optional(std::move(connections));
}
Mesh::Mesh(
          int drawMode,
          unsigned int shaderProgramId,
          glm::vec3& position,
          glm::vec3& rotation,
          glm::vec3& scale,
          std::vector<glm::vec3>& vertexes,
          std::vector<glm::vec4>& colors)
          : GeometricEntity(position, rotation, scale) {
  this->geometryChanged = true;
  this->uniformValuesUpdated = true;
  this->transformationCacheValid = false;
  this->visible = true;
  this->shaderProgramId = shaderProgramId;
  this->modelUniformId = glutils::queryUniformVariable(this->shaderProgramId, "Model");
  this->drawMode = drawMode;
  this->vertexes = std::move(vertexes);
  this->colors = std::move(colors);
  this->connections = std::optional<std::vector<unsigned int>>();
}
Mesh::Mesh(
          int drawMode,
          unsigned int shaderProgramId,
          std::vector<glm::vec3>& vertexes,
          std::vector<glm::vec4>& colors)
          : GeometricEntity() {
  this->geometryChanged = true;
  this->uniformValuesUpdated = true;
  this->transformationCacheValid = false;
  this->visible = true;
  this->shaderProgramId = shaderProgramId;
  this->modelUniformId = glutils::queryUniformVariable(this->shaderProgramId, "Model");
  this->drawMode = drawMode;
  this->vertexes = std::move(vertexes);
  this->colors = std::move(colors);
  this->connections = std::optional<std::vector<unsigned int>>();
}

unsigned int Mesh::getShaderProgramId() {
  return this->shaderProgramId;
}
int Mesh::getDrawMode() {
  return this->drawMode;
}
unsigned int Mesh::getVertexCount() {
  return this->vertexes.size();
}
unsigned int Mesh::getColorCount() {
  return this->colors.size();
}
std::vector<glm::vec3> Mesh::getVertexes() {
  return this->vertexes;
}
std::vector<glm::vec3>& Mesh::peekVertexes() {
  this->geometryChanged = true;
  return this->vertexes;
}
void Mesh::setVertexes(glm::vec3 vertexArray) {
  this->geometryChanged;
  this->vertexes;
}
void Mesh::setVertexes(glm::vec3&& vertexArray) {
  this->geometryChanged = true;
  this->vertexes;
}
void Mesh::setGeometryChanged(bool geometryChanged) {
  this->geometryChanged = geometryChanged;
}
bool Mesh::isGeometryChanged() {
  return this->geometryChanged;
}
void Mesh::resetGeometryChange() {
  this->geometryChanged = false;
}
std::vector<glm::vec4>& Mesh::getColors() {
  return this->colors;
}
std::optional<std::vector<unsigned int>>& Mesh::getConnections() {
  return this->connections;
}

void Mesh::essentialUniformUpdate() {
  //if (!transformationCacheValid) {    FIXME: Find a way to do update only if values were updated
    for (std::pair<const unsigned int, std::shared_ptr<Mesh::UniformUpdater>> idUpdaterPair
       : this->uniformUpdaters) {
      (*idUpdaterPair.second)();
    }
    this->transformationMatrix = glm::mat4(1.0);
    this->transformationMatrix = glm::translate(this->transformationMatrix, this->position);
    this->transformationMatrix = glm::rotate(this->transformationMatrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    this->transformationMatrix = glm::scale(this->transformationMatrix, this->scale);
    // TODO: Temporary fix for rotation, won't work with 3D
    this->transformationCacheValid = true;
  //}
  glUniformMatrix4fv(this->modelUniformId, 1, GL_FALSE, glm::value_ptr(this->transformationMatrix));
}

glm::mat4 Mesh::getTransformationMatrix() {
  return this->transformationMatrix;
}

unsigned int Mesh::bindUniformUpdater(std::shared_ptr<Mesh::UniformUpdater> updater) {
  unsigned int providedId = this->idProvider.provideId();
  this->uniformUpdaters.emplace(providedId, updater);
  return providedId;
}

void Mesh::unbindUniformUpdater(unsigned int uniformUpdaterId) {
  this->idProvider.freeId(uniformUpdaterId);
  this->uniformUpdaters.erase(uniformUpdaterId);
}

bool Mesh::isVisible() {
  return this->visible;
}

void Mesh::setVisible(bool visible) {
  this->visible = visible;
}
