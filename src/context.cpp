#include "context.hpp"

#include "scene.hpp"

Context::Context(Window window, Mouse mouse) {
  this->window = std::make_shared<Window>(window);
  this->mouse = std::make_shared<Mouse>(mouse);
}
Context::Context(std::shared_ptr<Window> window, std::shared_ptr<Mouse> mouse) {
  this->window = window;
  this->mouse = mouse;
}

void Context::setScene(std::shared_ptr<Scene> scene) {
  this->scene = std::make_optional(scene);
}

std::optional<std::shared_ptr<Scene>> Context::getScene() {
  return this->scene;
}

std::shared_ptr<Context::Window> Context::getWindow() {
  return this->window;
}

std::shared_ptr<Context::Mouse> Context::getMouse() {
  return this->mouse;
}
