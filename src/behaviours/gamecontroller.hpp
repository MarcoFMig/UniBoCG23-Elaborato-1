#pragma once

#include <vector>
#include <chrono>
#include <glm/vec3.hpp>

#include "../scene.hpp"
#include "../object.hpp"
#include "../shaderloader/shader.hpp"
#include "../inputhandler/simpleinputhandler.hpp"

#define __HIT_SCORE_INCREASE    25
#define __HEALTH_LOSS           25
#define __MAX_SHOOTING_ENEMIES  3

class GameController : public Behaviour {
  private:
    struct EntityPrototype {
      std::vector<glm::vec3> geometry;
      std::vector<glm::vec4> colors;
    };
    unsigned int pointsPerKill;
    unsigned int enemyNumber;
    unsigned int enemyLevels;
    unsigned int bgUniformUpdaterId;
    int victoryScore;
    int timePerMovement;
    float enemyStep;
    float initialEnemyX;
    bool gameOver;
    bool gameOverShown;

    EntityPrototype enemyPrototype;
    EntityPrototype playerPrototype;
    EntityPrototype projectilePrototype;

    std::chrono::high_resolution_clock::time_point enemyMovementTimepoint;

    SimpleInputHandler& inputHandler;
    std::shared_ptr<Object> background;
    std::unique_ptr<Shader::UniformVariable> backgroundSetter;
    std::shared_ptr<std::function<void()>> enableBGMode;
    std::shared_ptr<std::function<void()>> disableBGMode;
    std::function<void(unsigned int, int, int)> fireFunction;
    std::shared_ptr<std::function<void(std::shared_ptr<Object>, std::shared_ptr<Object>)>> projectileHitFunction;
    std::shared_ptr<std::function<void(std::shared_ptr<Object>, std::shared_ptr<Object>)>> enemyBulletFunction;
    std::vector<unsigned int> listenerIdentifiers;
    std::unordered_map<unsigned int, std::shared_ptr<Object>> enemies;
    std::vector<std::shared_ptr<Object>> projectiles;
    std::shared_ptr<Object> player;

    std::shared_ptr<Label> score;
    std::shared_ptr<Label> health;
    std::shared_ptr<Label> gameOverText;

    void makeMiscData();
    void makeEnemyData();
    std::shared_ptr<Object> makeEnemy(glm::vec3 position);
    void makePlayerData();
    void makeBackground();
    void makeEnemies();
    void makePlayer(glm::vec3 position);
    void makeText();
    void moveEnemies();
    void triggerGameOver(bool isVictory);
    unsigned int getRandomEnemy();
    void makeEnemyShoot(unsigned int enemyNumber);
  public:
    GameController(std::shared_ptr<Object> objectReference,
      std::shared_ptr<Scene> sceneReference);
    void setup() override;
    void frameCycle() override;
    void end() override;
};
