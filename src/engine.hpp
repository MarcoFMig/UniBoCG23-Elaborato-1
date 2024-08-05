#pragma once

#include "scene.hpp"
#include "context.hpp"

class Engine {
  private:
    //Context context;


    void setupGl(int argv, char* argc[]);

    Engine() = default;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    static void graphicsLoopCallback();

    void graphicsLoop();
  public:
    static Engine& getInstance();
    void initialize();
    void start();
    void loadScene();
    void resetScene();
    void setContext();
    void resetContext();
};
