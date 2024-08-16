#pragma once

#include <string>
#include <memory>
#include <functional>
#include "mesh.hpp"

/**
 * Class that represents a scene containing simple geometric elements.
 */
class GeometricScene {
  public:
    /**
     * @returns                     all elements inside the scene
     */
    virtual std::vector<std::shared_ptr<Mesh>> getMeshes() = 0;
    /**
     * @returns                     number of elements in the scene
     */
    virtual unsigned int getElementCount() = 0;
    /**
     * @returns                     complete number of vertexes contained inside the scene.
     */
    virtual unsigned int getVertexCount() = 0;
    /**
     * Adds a scene change listener that gets updated whenever something is updated inside the scene
     * @param sceneChangeListener   scene change listener to be added
     * @returns                     identifier of the listener
     */
    virtual unsigned int addSceneChangeListener(std::function<void()> sceneChangeListener) = 0;
    /**
     * Removes a listener given an identifier.
     * @param listenerId            the listener's identifier to be removed
     */
    virtual void removeSceneChangeListener(unsigned int listenerId) = 0;
    /**
     * Destroys geometricscene.
     */
    virtual ~GeometricScene() = default;
};
