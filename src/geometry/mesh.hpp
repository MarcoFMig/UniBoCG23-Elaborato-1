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

/**
 * Class representing a geometric entity composed of a geometry to be displayed in the scene.
 */
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
    /**
     * Constructs a new mesh object.
     * @param drawMode            draw mode of the current mesh
     * @param shaderProgramId     shader program id to be used for this mesh
     * @param position            mesh's starting position
     * @param rotation            mesh's starting rotation
     * @param scale               mesh's starting scale
     * @param vertexes            mesh's geometry
     * @param colors              mesh's color for each vertex
     * @param connections         mesh's vertexes connections
     */
    Mesh(
      int drawMode,
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    /**
     * Constructs a new mesh object.
     * @param drawMode            draw mode of the current mesh
     * @param shaderProgramId     shader program id to be used for this mesh
     * @param vertexes            mesh's geometry
     * @param colors              mesh's color for each vertex
     * @param connections         mesh's vertexes connections
     */
    Mesh(
      int drawMode,
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    /**
     * Constructs a new mesh object.
     * @param drawMode            draw mode of the current mesh
     * @param shaderProgramId     shader program id to be used for this mesh
     * @param position            mesh's starting position
     * @param rotation            mesh's starting rotation
     * @param scale               mesh's starting scale
     * @param vertexes            mesh's geometry
     * @param colors              mesh's color for each vertex
     */
    Mesh(
      int drawMode,
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);
    /**
     * Constructs a new mesh object.
     * @param drawMode            draw mode of the current mesh
     * @param shaderProgramId     shader program id to be used for this mesh
     * @param vertexes            mesh's geometry
     * @param colors              mesh's color for each vertex
     */
    Mesh(
      int drawMode,
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);

    /**
     * @returns             the current mesh's draw mode
     */
    virtual int getDrawMode();
    /**
     * @returns             the currrent mesh's shader program identifier
     */
    virtual unsigned int getShaderProgramId();
    /**
     * @returns             the current mesh's vertex count
     */
    virtual unsigned int getVertexCount();
    /**
     * @returns             a NON PROTECTED list containing every vertex of the mesh
     */
    virtual std::vector<glm::vec3>& peekVertexes();
    /**
     * @returns             a defensive copy of the list containing every vertex of the mesh
     */
    virtual std::vector<glm::vec3> getVertexes();
    /**
     * Updates the current mesh's geometry.
     * @param vertexArray   an array containing all vertexes
     */
    virtual void setVertexes(glm::vec3 vertexArray);
    /**
     * Updates the current mesh's geometry.
     * @param vertexArray   an array containing all vertexes
     */
    virtual void setVertexes(glm::vec3&& vertexArray);
    /**
     * @returns             color count for the mesh
     */
    virtual unsigned int getColorCount();
    /**
     * @returns             a NON PROTECTED list containing every color of the mesh
     */
    virtual std::vector<glm::vec4>& getColors();
    /**
     * @returns             a NON PROTECTED list containing every connection of the mesh
     */
    virtual std::optional<std::vector<unsigned int>>& getConnections();

    virtual void setGeometryChanged(bool geometryChanged);
    virtual bool isGeometryChanged();
    virtual void resetGeometryChange();

    /**
     * @returns the mesh's current transformation matrix.
     */
    virtual glm::mat4 getTransformationMatrix();

    /**
     * Adds an uniform updater to be called before a draw call for this specific mesh.
     * @param updater       the updater function
     * @returns             the updater's identifier
     */
    virtual unsigned int bindUniformUpdater(std::shared_ptr<Mesh::UniformUpdater> updater);
    /**
     * Removes an uniform updater.
     * @param updater       the updater's identifier
     */
    virtual void unbindUniformUpdater(unsigned int uniformUpdaterId);
    /**
     * Causes all essential uniforms to update.
     */
    virtual void essentialUniformUpdate();

    /**
     * @returns             true if the mesh should be visible
     */
    virtual bool isVisible();
    /**
     * Determines if the mesh should be visible.
     * @param visible       wether or not the object should be visible
     */
    virtual void setVisible(bool visible);
    /**
     * Destroys the mesh.
     */
    virtual ~Mesh() = default;
};
