#pragma once

#include <list>
#include <memory>
#include <functional>
#include <unordered_map>

#include "geometricscene.hpp"

/**
 * Class representing a renderer for a scene.
 */
class MeshRenderer {
  private:
    struct VertexBufferElement {
      unsigned int startIndex;
      unsigned int length;
      unsigned int shaderId;
      std::shared_ptr<Mesh> meshReference;
    };
    using MeshIdMap = std::unordered_map<unsigned int, std::vector<VertexBufferElement>>;
    std::shared_ptr<GeometricScene> sceneReference;
    int renderMode;
    unsigned int vaoId;
    bool vaoReady;
    unsigned int vboId;
    bool vboReady;
    unsigned int shaderId;
    unsigned int projectionShaderId;
    bool projectionMatUpdated;
    glm::mat4 projectionMat;
    std::unordered_map<int, MeshIdMap> drawReferenceMap;
    std::unordered_map<unsigned int, std::list<std::shared_ptr<std::function<void()>>>> uniformUpdates;
  public:
    /**
     * Constructs a mesh renderer.
     * @param shaderId            shader program's identifier to be used during the rendering process
     * @param projectionShaderId  the uniform variable's identifier representing the projection matrix
     * @param renderMode          vertex storing mode to be used when allocating buffer
     */
    MeshRenderer(unsigned int shaderId, unsigned int projectionShaderId, int renderMode);
    /**
     * Recieves a function that is going to be called before every draw call in order to update uniforms.
     * @param shaderId            shader identifier where the uniform resides
     * @param action              function containing action to be executed to udpate uniforms
     */
    void enqueueUniformUpdate(unsigned int shaderId, std::shared_ptr<std::function<void()>> action);
    /**
     * Allocates buffer in the GPU in order to store geometry and colors.
     * @param scene               scene containing all geometry
     */
    void prepareBuffers(std::shared_ptr<GeometricScene> scene);
    /**
     * Binds a scene to the renderer in order to automatically handle scene content updates.
     * @param scene               scene to keep track of
     */
    void bindScene(std::shared_ptr<GeometricScene> scene);
    /**
     * Causes the rendering of the scene.
     */
    void render();

    /**
     * Updates the projection matrix stored in the renderer.
     * @param projectionMat       projection matrix
     */
    void updateProjection(glm::mat4& projectionMat);
    /**
     * Updates the shader identifier to be used during the rendering.
     * @param shaderId            the new shader's identifier
     */
    void setShaderId(unsigned int shaderId);
    /**
     * @returns the shader identifier used in the rendering
     */
    unsigned int getShaderId();
    /**
     * Destroys the renderer
     */
    virtual ~MeshRenderer();
};
