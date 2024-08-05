#pragma once

#include <string>
#include <memory>
#include <functional>
#include "mesh.hpp"

class GeometricScene {
  public:
    virtual std::vector<std::shared_ptr<Mesh>> getMeshes() = 0;
    virtual unsigned int getElementCount() = 0;
    virtual unsigned int getVertexCount() = 0;
    virtual unsigned int addSceneChangeListener(std::function<void()>) = 0;
    virtual void removeSceneChangeListener(unsigned int listenerId) = 0;
    virtual ~GeometricScene() = default;
};
