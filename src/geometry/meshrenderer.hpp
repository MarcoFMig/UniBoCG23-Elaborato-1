#pragma once

#include <list>
#include <memory>
#include <functional>
#include <unordered_map>

#include "geometricscene.hpp"

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
    MeshRenderer(unsigned int shaderId, unsigned int projectionShaderId, int renderMode);
    void enqueueUniformUpdate(unsigned int shaderId, std::shared_ptr<std::function<void()>> action);
    void prepareBuffers(std::shared_ptr<GeometricScene> scene);
    void bindScene(std::shared_ptr<GeometricScene> scene);
    void render();

    void updateProjection(glm::mat4& projectionMat);
    void setShaderId(unsigned int shaderId);
    unsigned int getShaderId();
    virtual ~MeshRenderer();
};
