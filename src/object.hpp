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

/**
 * Represents a object to be used inside a scene.
 */
class Object : public Mesh {
  public:
    /**
     * Events that can occurr within the object.
     */
    enum EventType {
      /**
       * Every time an object is deleted from a scene.
       */
      object_deletion,
      /**
       * Every time a child object is attached.
       */
      child_addition,
      /**
       * Every time an object is deleted.
       */
      child_deletion,
      /**
       * Every time a parent is added to the object.
       */
      parent_addition,
      /**
       * Every time a parent is deleted from the object.
       */
      parent_deletion
    };
    /**
     * @deprecated
     */
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
    /**
     * Constructs an object instance.
     * @param drawMode        the draw mode of this object's mesh
     * @param shaderProgramId the shader program's identifier of this object's mesh
     * @param position        a vec3 that represents the object's starting position
     * @param rotation        a vec3 that represents the object's starting rotation
     * @param scale           a vec3 that represents the object's starting scale
     * @param vertexes        a vector of vec3 representing the object's normalized geometry
     * @param colors          a vector of vec4 representing the object's color for each vertex
     * @param connections     a vector of unsigned ints representing the connections inside the object
     */
    Object(
      int drawMode,
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    /**
     * Constructs an object instance.
     * @param drawMode        the draw mode of this object's mesh
     * @param shaderProgramId the shader program's identifier of this object's mesh
     * @param vertexes        a vector of vec3 representing the object's normalized geometry
     * @param colors          a vector of vec4 representing the object's color for each vertex
     * @param connections     a vector of unsigned ints representing the connections inside the object
     */
    Object(
      int drawMode,
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    /**
     * Constructs an object instance.
     * @param drawMode        the draw mode of this object's mesh
     * @param shaderProgramId the shader program's identifier of this object's mesh
     * @param position        a vec3 that represents the object's starting position
     * @param rotation        a vec3 that represents the object's starting rotation
     * @param scale           a vec3 that represents the object's starting scale
     * @param vertexes        a vector of vec3 representing the object's normalized geometry
     * @param colors          a vector of vec4 representing the object's color for each vertex
     */
    Object(
      int drawMode,
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);
    /**
     * Constructs an object instance.
     * @param drawMode        the draw mode of this object's mesh
     * @param shaderProgramId the shader program's identifier of this object's mesh
     * @param vertexes        a vector of vec3 representing the object's normalized geometry
     * @param colors          a vector of vec4 representing the object's color for each vertex
     */
    Object(
      int drawMode,
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);
    /**
     * Constructs an object instance.
     * @param shaderProgramId the shader program's identifier of this object's mesh
     * @param position        a vec3 that represents the object's starting position
     * @param rotation        a vec3 that represents the object's starting rotation
     * @param scale           a vec3 that represents the object's starting scale
     * @param vertexes        a vector of vec3 representing the object's normalized geometry
     * @param colors          a vector of vec4 representing the object's color for each vertex
     * @param connections     a vector of unsigned ints representing the connections inside the object
     */
    Object(
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    /**
     * Constructs an object instance.
     * @param shaderProgramId the shader program's identifier of this object's mesh
     * @param vertexes        a vector of vec3 representing the object's normalized geometry
     * @param colors          a vector of vec4 representing the object's color for each vertex
     * @param connections     a vector of unsigned ints representing the connections inside the object
     */
    Object(
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors,
      std::vector<unsigned int>& connections);
    /**
     * Constructs an object instance.
     * @param shaderProgramId the shader program's identifier of this object's mesh
     * @param position        a vec3 that represents the object's starting position
     * @param rotation        a vec3 that represents the object's starting rotation
     * @param scale           a vec3 that represents the object's starting scale
     * @param vertexes        a vector of vec3 representing the object's normalized geometry
     * @param colors          a vector of vec4 representing the object's color for each vertex
     */
    Object(
      unsigned int shaderProgramId,
      glm::vec3& position,
      glm::vec3& rotation,
      glm::vec3& scale,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);
    /**
     * Constructs an object instance.
     * @param shaderProgramId the shader program's identifier of this object's mesh
     * @param vertexes        a vector of vec3 representing the object's normalized geometry
     * @param colors          a vector of vec4 representing the object's color for each vertex
     */
    Object(
      unsigned int shaderProgramId,
      std::vector<glm::vec3>& vertexes,
      std::vector<glm::vec4>& colors);

    /**
     * Binds an event listener to this object.
     * @param eventListener the event listener to be bound to this object
     */
    unsigned int bindEventListener(std::function<void(Object::EventType, std::shared_ptr<Object>)>&& eventListener);
    /**
     * Unbinds an event listener given to this object given an identifier.
     * @param listenerId    the identifier of the listener to be removed
     */
    void unbindEventListener(unsigned int listenerId);
    /**
     * Adds a child to this object.
     * @param child         the children to be added
     */
    void addChild(std::shared_ptr<Object> child);
    /**
     * Removes a child that has the same identifier of this object's children.
     * @param child         the object's child to be removed
     */
    void removeChild(std::shared_ptr<Object> child);
    /**
     * @returns the object's parent
     */
    std::optional<std::shared_ptr<Object>> getParent();
    /**
     * @returns the object's children as an unordered map
     */
    std::unordered_map<unsigned int, std::shared_ptr<Object>> getChildren();
    /**
     * @returns an optional that might contain the object's identifier, if assigned
     */
    std::optional<unsigned int> getIdentifier();
    /**
     * @returns the object's name
     */
    std::string getName();
    /**
     * Sets the object's name
     * @param name          string containing object's name
     */
    void setName(std::string name);
    /**
     * adds an identifier to the object.
     * @param identifier    an optional containing the identifier
     */
    void setIdentifier(std::optional<unsigned int> identifier);
    /**
     * adds an identifier to the object.
     * @param identifier    the object's identifier
     */
    void setIdentifier(unsigned int identifier);
    /**
     * Removes the object's identifier
     */
    void unsetIdentifier();
    /**
     * Destroys the object.
     */
    virtual ~Object() = default;
};
