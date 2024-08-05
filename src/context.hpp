#pragma once

#include <string>

#include "scene.hpp"

class Context {
  public:
    struct Window {
      int id;
      std::string title;
      int currentHeight;
      int currentWidth;
      float ratioedHeight;
      float ratioedWidth;
      float aspectRatio;
      float aspectRatioedHeight;
      float aspectRatioedWidth;
    };
    struct Mouse {
      int currentX;
      int currentY;
    };
  private:
    std::optional<std::shared_ptr<Scene>> scene;
    std::shared_ptr<Window> window;
    std::shared_ptr<Mouse> mouse;
  public:
    Context(Window window, Mouse mouse);
    Context(std::shared_ptr<Window> window, std::shared_ptr<Mouse> mouse);
    std::optional<std::shared_ptr<Scene>> getScene();
    void setScene(std::shared_ptr<Scene> scene);
    std::shared_ptr<Window> getWindow();
    std::shared_ptr<Mouse> getMouse();
};
