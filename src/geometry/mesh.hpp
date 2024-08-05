#pragma once

#include <any>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <unordered_map>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "geometricentity.hpp"
#include "../lightweightutils/idprovider.hpp"

class Mesh : public GeometricEntity {
  public:
    using UniformUpdater = std::function<void()>;
  private:
    IDProvider idProvider;
    bool visible;
    bool geometryChanged;
    int drawMode;
    unsigned int shaderProgramId;
    unsigned int modelUniformId;
    bool uniformValuesUpdated;
    std::vector<glm::vec3> vertexes;
    std::vector<glm::vec4> colors;
    std::optional<std::vector<unsigned int>> connections;
    std::unordered_map<unsigned int,  std::shared_ptr<Mesh::UniformUpdater>> uniformUpdaters;
  protected:
    glm::mat4 transformationMatrix;
    bool transformationCacheValid;
    Mesh() = default;
  public:
    Mesh(
      int drawMode,
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    Mesh(
      int drawMode,
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    Mesh(
      int drawMode,
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);
    Mesh(
      int drawMode,
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);

    virtual int getDrawMode();
    virtual unsigned int getShaderProgramId();
    virtual unsigned int getVertexCount();
    virtual std::vector<glm::vec3>& peekVertexes();
    virtual std::vector<glm::vec3> getVertexes();
    virtual void setVertexes(glm::vec3 vertexArray);
    virtual void setVertexes(glm::vec3&& vertexArray);
    virtual unsigned int getColorCount();
    virtual std::vector<glm::vec4>& getColors();
    virtual std::optional<std::vector<unsigned int>>& getConnections();
    virtual void setGeometryChanged(bool geometryChanged);
    virtual bool isGeometryChanged();
    virtual void resetGeometryChange();

    virtual glm::mat4 getTransformationMatrix();

    virtual unsigned int bindUniformUpdater(std::shared_ptr<Mesh::UniformUpdater> updater);
    virtual void unbindUniformUpdater(unsigned int uniformUpdaterId);
    virtual void essentialUniformUpdate();

    virtual bool isVisible();
    virtual void setVisible(bool visible);
    virtual ~Mesh() = default;
};
