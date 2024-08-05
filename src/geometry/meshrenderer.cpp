#include "meshrenderer.hpp"

#include <map>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>
#include <gl/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "geometricscene.hpp"

#define __ELEMENTS_PER_POINT 7
#define __DIMENSIONS 3
#define __COLOR_VALUES 4

MeshRenderer::MeshRenderer(unsigned int shaderId, unsigned int projectionShaderId, int renderMode) {
  this->shaderId = shaderId;
  this->projectionShaderId = projectionShaderId;
  this->renderMode = renderMode;
  this->projectionMatUpdated = true;
  this->vaoReady = false;
  this->vboReady = false;
}
void MeshRenderer::prepareBuffers(std::shared_ptr<GeometricScene> scene) {
  // TODO: Add preallocation to speed up VBO generation;
  this->sceneReference = scene;
  std::map<int, std::map<unsigned int, std::vector<unsigned int>>> drawIndexShaderMap;
  std::vector<float> vertexBuffer;
  this->drawReferenceMap.clear();

  // Separating meshes by draw modes
  for (unsigned int index = 0; index < this->sceneReference.get()->getElementCount(); index++) {
    int meshDrawMode = this->sceneReference.get()->getMeshes()[index].get()->getDrawMode();
    int meshShaderId = this->sceneReference.get()->getMeshes()[index].get()->getShaderProgramId();
    drawIndexShaderMap[meshDrawMode][meshShaderId].push_back(index);
  }
  if (!this->vaoReady) {
    glGenVertexArrays(1, &this->vaoId);
    this->vaoReady = true;
  }
  glBindVertexArray(this->vaoId);
  unsigned int lastElementPosition = 0;
  unsigned int lastElementSize = 0;
  // VBO creation for each draw type
  for (std::pair<const int, std::map<unsigned int, std::vector<unsigned int>>>& drawTypeMap : drawIndexShaderMap) {
      std::vector<VertexBufferElement> elements;
      unsigned int preallocSpace = 0;
      elements.reserve(scene.get()->getElementCount());
    for (std::pair<const unsigned int, std::vector<unsigned int>>& shaderTypeMap : drawTypeMap.second) {
      for (unsigned int meshIndex : shaderTypeMap.second) {
        VertexBufferElement element;
        element.startIndex = lastElementSize + lastElementPosition;
        lastElementPosition = element.startIndex;
        element.length = this->sceneReference.get()->getMeshes()[meshIndex].get()->peekVertexes().size();
        element.meshReference = this->sceneReference.get()->getMeshes()[meshIndex];
        element.shaderId = shaderTypeMap.first;
        lastElementSize = element.length;
        elements.push_back(std::move(element));
        for (unsigned int vertexVectorIndex = 0; vertexVectorIndex < this->sceneReference.get()->getMeshes()[meshIndex].get()->getVertexCount();
          vertexVectorIndex ++) {
          for (unsigned short dimensionIndex = 0; dimensionIndex < __DIMENSIONS; dimensionIndex ++) {
            vertexBuffer.push_back(this->sceneReference.get()->getMeshes()[meshIndex].get()->getVertexes()[vertexVectorIndex][dimensionIndex]);
          }
          for (unsigned short colorSectionIndex = 0; colorSectionIndex < __COLOR_VALUES; colorSectionIndex ++) {
            vertexBuffer.push_back(this->sceneReference.get()->getMeshes()[meshIndex].get()->getColors()[vertexVectorIndex][colorSectionIndex]);
          }
        }
      }
     this->drawReferenceMap[drawTypeMap.first][shaderTypeMap.first] = std::move(elements);
    }
  }
  if (!this->vboReady) {
    glGenBuffers(1, &this->vboId);
    this->vboReady = true;
  }
  glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
  glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), this->renderMode);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, __ELEMENTS_PER_POINT * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, __ELEMENTS_PER_POINT * sizeof(float), (void *)(3 * sizeof(float)));
}

void MeshRenderer::bindScene(std::shared_ptr<GeometricScene> sceneToBind) {
  sceneToBind->addSceneChangeListener([this, &sceneToBind]() {
    this->prepareBuffers(sceneToBind);
  });
}

void MeshRenderer::render() {
  glBindVertexArray(this->vaoId);
  glBindBuffer(GL_ARRAY_BUFFER, this->vboId);

  bool firstDraw = true;
  unsigned int lastShaderValue;

  for (const std::pair<int, std::unordered_map<unsigned int, std::vector<VertexBufferElement>>>& drawTypeMap : this->drawReferenceMap) {
    for (const std::pair<unsigned int, std::vector<VertexBufferElement>>& shaderTypeMap : drawTypeMap.second) {
      if (firstDraw) {
        firstDraw = false;
        lastShaderValue = shaderTypeMap.first;
        //printf("A \tUSing program %i\n", lastShaderValue);
        glUseProgram((GLuint) lastShaderValue);
      }
      if (lastShaderValue != shaderTypeMap.first) {
        lastShaderValue = shaderTypeMap.first;
        //printf("A \tUSing program %i\n", lastShaderValue);
        glUseProgram((GLuint) lastShaderValue);
      }
      for (std::shared_ptr<std::function<void()>> uniformUpdate : this->uniformUpdates[lastShaderValue]) {
        (*uniformUpdate)();
      }
      this->uniformUpdates[lastShaderValue].clear();
      for (const VertexBufferElement& vertexEntry : shaderTypeMap.second) {
        if (vertexEntry.meshReference.get()->isVisible()) {
          vertexEntry.meshReference.get()->essentialUniformUpdate();
          glDrawArrays(drawTypeMap.first, vertexEntry.startIndex, vertexEntry.length);
        }
      }
    }
  }
}

void MeshRenderer::setShaderId(unsigned int shaderId) {
  this->shaderId = shaderId;
}
unsigned int MeshRenderer::getShaderId() {
  return this->shaderId;
}

void MeshRenderer::enqueueUniformUpdate(unsigned int shaderId, std::shared_ptr<std::function<void()>> action) {
  this->uniformUpdates[shaderId].push_back(action);
}

void MeshRenderer::updateProjection(glm::mat4& projectionMat) {
  if (this->projectionMatUpdated) {
    glUniformMatrix4fv(this->projectionShaderId, 1, GL_FALSE, glm::value_ptr(projectionMat));
    //glUniform2f(this->projectionShaderId, width, height);
  }
}

MeshRenderer::~MeshRenderer() {
  if (this->vboReady) {
    glDeleteBuffers(1, (GLuint*) &this->vboId);
  }
  if (this->vaoReady) {
    glDeleteVertexArrays(1, (GLuint*) &this->vaoId);
  }
}
