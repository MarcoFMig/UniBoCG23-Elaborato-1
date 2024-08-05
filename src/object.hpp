#pragma once

#include "geometry/mesh.hpp"

#include <list>
#include <memory>
#include <string>
#include <optional>
#include <functional>
#include <unordered_map>
#include "lightweightutils/idprovider.hpp"
#include "lightweightutils/scheduling/tickable.hpp"

class Object : public Mesh {
  public:
    enum EventType {
      object_deletion,
      child_addition,
      child_deletion,
      parent_addition,
      parent_deletion
    };
    enum MovementReference {
      world,
      local,
      without_children,
      with_children
    };
  private:
    std::string name;
    std::optional<unsigned int> identifier;
    IDProvider idProvider;
    std::optional<std::shared_ptr<Object>> parent;
    std::unordered_map<unsigned int, std::shared_ptr<Object>> children;
    std::unordered_map<unsigned int, std::function<void(Object::EventType, std::shared_ptr<Object>)>> listeners;

    void triggerEvent(Object::EventType event, std::shared_ptr<Object> subject);
    void setParent(std::optional<std::shared_ptr<Object>> parent);
  public:
    Object(
      int drawMode,
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    Object(
      int drawMode,
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    Object(
      int drawMode,
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);
    Object(
      int drawMode,
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);
    Object(
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    Object(
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    Object(
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);
    Object(
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);

    unsigned int bindEventListener(std::function<void(Object::EventType, std::shared_ptr<Object>)>&& eventListener);
    void unbindEventListener(unsigned int listenerId);
    void addChild(std::shared_ptr<Object> child);
    void removeChild(std::shared_ptr<Object> child);
    std::optional<std::shared_ptr<Object>> getParent();
    std::unordered_map<unsigned int, std::shared_ptr<Object>> getChildren();
    std::optional<unsigned int> getIdentifier();
    std::string getName();
    void setName(std::string name);
    void setIdentifier(std::optional<unsigned int> identifier);
    void setIdentifier(unsigned int identifier);
    void unsetIdentifier();
    virtual ~Object() = default;
};
