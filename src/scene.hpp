#pragma once

#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <functional>
#include <unordered_map>
#include <gl/glew.h>
#include <glm/mat4x4.hpp>

#include "shaderloader/shader.hpp"
#include "geometry/label.hpp"
#include "object.hpp"
#include "subsystem.hpp"
#include "geometry/ui.hpp"
#include "behaviours/ibehaviour.hpp"
#include "geometry/geometricscene.hpp"
#include "lightweightutils/idprovider.hpp"

class Scene;

class Behaviour : public IBehaviour {
  protected:
    std::shared_ptr<Object> objectReference;
    std::shared_ptr<Scene> sceneReference;
    Behaviour(std::shared_ptr<Object> objectReference, std::shared_ptr<Scene> sceneReference);
  public:
    virtual void setup() override {};
    virtual void frameCycle() override {};
    virtual void end() override {};
    virtual ~Behaviour() = default;
};

class Scene : public GeometricScene, public Tickable {
  public:
    using BehaviourMap = std::unordered_map<unsigned int, std::shared_ptr<IBehaviour>>;
    using ObjectAndBehavioursPair = std::pair<std::shared_ptr<Object>, BehaviourMap>;
    using ObjectMap = std::unordered_map<unsigned int, ObjectAndBehavioursPair>;
    using SharedObjectMap = std::shared_ptr<ObjectMap>;
  private:
    std::string sceneName;
    glm::mat4 world;

    std::shared_ptr<Shader> mainShader;
    std::list<std::shared_ptr<Label>> text;
    IDProvider idProvider;
    unsigned int sceneIdentifier;
    SharedObjectMap objectMapRef;
    float ratioedWindowHeight;
    float ratioedWindowWidth;
    int currentWidth;
    int currentHeight;
  
    std::list<unsigned int> enqueuedRemoval;
    std::unordered_map<unsigned int, std::function<void()>> registeredChangeListeners;
    std::unordered_map<Subsystem::SubsystemType, std::shared_ptr<Subsystem>> subsystems;
    
    BehaviourMap& retrieveBehaviourMap(unsigned int objectId);
    void updateSubsystems(Subsystem::UpdateType reason,
      std::shared_ptr<Object> targetObject,
      std::shared_ptr<IBehaviour> responsible);
    void triggerSceneChange();
    void objectProbe(Object::EventType event, std::shared_ptr<Object> responsible);
  public:
    Scene(std::shared_ptr<Shader> mainShader, glm::mat4 world);
    Scene(std::shared_ptr<Shader> mainShader);

    void setName(std::string newName);
    std::string getName();

    unsigned int addObject(Object object);
    unsigned int addObject(Object&& object);
    unsigned int addObject(std::shared_ptr<Object> objectRef);

    unsigned int attachBehaviour(unsigned int objectId, std::shared_ptr<IBehaviour> behaviourRef);

    unsigned int attachBehaviour(std::shared_ptr<Object> object, std::shared_ptr<IBehaviour> behaviourRef);

    void removeObject(unsigned int objectId);
    void removeObject(std::shared_ptr<Object> objectRef);

    void detachBehaviour(unsigned int objectId, unsigned int behaviourId);
    void detachBehaviour(unsigned int objectId, std::shared_ptr<IBehaviour> behaviourRef);

    void detachBehaviour(std::shared_ptr<Object> object, unsigned int behaviourId);
    void detachBehaviour(std::shared_ptr<Object> object, std::shared_ptr<IBehaviour> behaviourRef);

    std::shared_ptr<Shader> getMainShader();

    void setText(std::list<std::shared_ptr<Label>> text);
    void addText(std::shared_ptr<Label> text);
    std::list<std::shared_ptr<Label>> getText();
    std::list<std::shared_ptr<Label>>& peekText();
    void clearText();

    void updateWindowData(float ratioedWindowHeight,
      float ratioedWindowWidth,
      int currentWidth,
      int currentHeight);
    
    float getRatioedWindowHeight();
    float getRatioedWindowWidth();
    int getCurrentWidth();
    int getCurrentHeight();

    void updateWorld(glm::mat4 world);

    unsigned int getElementCount() override;
    unsigned int getVertexCount() override;
    std::vector<std::shared_ptr<Mesh>> getMeshes() override;
    unsigned int addSceneChangeListener(std::function<void()>) override;
    void removeSceneChangeListener(unsigned int listenerId) override;
    void tick() override;
};
