#include "gamecontroller.hpp"

#include <cmath>
#include <memory>
#include <random>
#include <stdexcept>
#include <functional>
#include <gl/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../initdata.hpp"
#include "projectile.hpp"
#include "../logger/logger.hpp"
#include "data/firstgamedata.hpp"
#include "charactercontroller.hpp"
#include "physics/rectangularcollider.hpp"
#include "../simplemath/hermitecurve.hpp"
#include "../simplemath/hermiteparser.hpp"
#include "../inputhandler/simpleinputhandler.hpp"

GameController::GameController(std::shared_ptr<Object> objectReference,
  std::shared_ptr<Scene> sceneReference) : Behaviour(objectReference, sceneReference),
  inputHandler(SimpleInputHandler::getInstance()) {
  this->enemyNumber = 5;
  this->enemyLevels = 2;
  this->victoryScore = (this->enemyNumber * this->enemyLevels) * __HIT_SCORE_INCREASE;
  this->pointsPerKill = 25;
  this->gameOver = false;
  this->timePerMovement = 1000;
  this->gameOverShown = false;
  this->disableBGMode = std::make_shared<std::function<void()>>([this]() {
      glUniform1i(this->backgroundSetter->id, (GLint) 0);
    }
  );
  this->enableBGMode = std::make_shared<std::function<void()>>([this]() {
      glUniform1i(this->backgroundSetter->id, (GLint) 1);
    }
  );
  this->projectileHitFunction = std::make_shared<
  std::function<void(std::shared_ptr<Object>, std::shared_ptr<Object>)>>(
    [this](std::shared_ptr<Object> objectA, std::shared_ptr<Object> objectB) {
      std::shared_ptr<Object> hitEnemy;
      bool enemyPresent = false;
      std::shared_ptr<Object> firedProjectile;
      bool projectilePresent = false;
      if (objectA->getName() == "enemy") {
        hitEnemy = objectA;
        enemyPresent = true;
      } else if (objectA->getName() == "projectile") {
        firedProjectile = objectA;
        projectilePresent = true;
      }
      if (objectB->getName() == "enemy") {
        hitEnemy = objectB;
        enemyPresent = true;
      } else if (objectB->getName() == "projectile") {
        firedProjectile = objectB;
        projectilePresent = true;
      }
      if (!enemyPresent || !projectilePresent) {
        return;
      }
      unsigned int enemyId;
      for (std::pair<unsigned int, std::shared_ptr<Object>> numberAndEnemy : this->enemies) {
        if (numberAndEnemy.second->getIdentifier().value() == hitEnemy->getIdentifier().value()) {
          enemyId = numberAndEnemy.first;
          break;
        }
      }
      this->enemies.erase(enemyId);
      this->sceneReference->removeObject(hitEnemy);
      this->sceneReference->removeObject(firedProjectile);
      //Remove enemies fromlist
      FirstGameData::getInstance().setScore(
        FirstGameData::getInstance().getScore() + __HIT_SCORE_INCREASE);
      if (FirstGameData::getInstance().getScore() == this->victoryScore) {
        this->triggerGameOver(true);
      }
    }
  );
  this->enemyBulletFunction = std::make_shared<
  std::function<void(std::shared_ptr<Object>, std::shared_ptr<Object>)>>(
    [this](std::shared_ptr<Object> objectA, std::shared_ptr<Object> objectB) {
      std::shared_ptr<Object> hitPlayer;
      bool playerPresent = false;
      std::shared_ptr<Object> firedProjectile;
      bool projectilePresent = false;
      if (objectA->getName() == "player") {
        hitPlayer = objectA;
        playerPresent = true;
      } else if (objectA->getName() == "projectile") {
        firedProjectile = objectA;
        projectilePresent = true;
      }
      if (objectB->getName() == "player") {
        hitPlayer = objectB;
        playerPresent = true;
      } else if (objectB->getName() == "projectile") {
        firedProjectile = objectB;
        projectilePresent = true;
      }
      if (!playerPresent || !projectilePresent) {
        return;
      }
      this->sceneReference->removeObject(firedProjectile);
      FirstGameData::getInstance().setHealth(
        FirstGameData::getInstance().getHealth() - __HEALTH_LOSS);
      if (FirstGameData::getInstance().getHealth() <= 0) {
        this->triggerGameOver(false);
      }
    }
  );
  this->fireFunction = [this](unsigned int key, int mouseX, int mouseY) {
    std::vector<glm::vec3> projectileGeometry = projectilePrototype.geometry;
    std::vector<glm::vec4> projectileColors = projectilePrototype.colors;
    std::shared_ptr<Object> projectile = std::make_shared<Object>(GL_TRIANGLE_FAN,
      this->sceneReference->getMainShader()->getId(), projectileGeometry, projectileColors);
    projectile->setScale(glm::vec3(10.0f, 10.0f, 10.0f));
    glm::vec3 positionCopy = this->player->getPosition();
    projectile->setPosition(positionCopy);
    projectile->setName("projectile");
    this->sceneReference->addObject(projectile);
    std::shared_ptr<RectangularCollider> rectangularCollider =
      std::make_shared<RectangularCollider>(glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f));
    rectangularCollider->registerCollisionListener(this->projectileHitFunction);
    this->sceneReference->attachBehaviour(
      projectile, std::make_shared<Projectile>(projectile, this->sceneReference, 1000.0f));
    this->sceneReference->attachBehaviour(
      projectile, rectangularCollider);
  };
  this->makeEnemyData();
  this->makePlayerData();
  this->makeMiscData();
  this->makeText();
  FirstGameData::getInstance().setHealth(100);
  FirstGameData::getInstance().setScore(0);
}

void GameController::setup() {
  this->makeBackground();
  this->makeEnemies();
  this->inputHandler.registerKeyDownListener(' ', this->fireFunction);
  this->makePlayer(glm::vec3(DEFAULT_WINDOW_WIDTH / 2.0f, 235.0f, 1.0f));
  this->enemyMovementTimepoint = std::chrono::high_resolution_clock::now();
}

void GameController::frameCycle() {
  this->health->setText(std::string("Health: ") + std::to_string(FirstGameData::getInstance().getHealth()));
  this->score->setText(std::string("Score: ") + std::to_string(FirstGameData::getInstance().getScore()));
  if (this->gameOver) {
    if (!this->gameOverShown) {
      this->sceneReference->addText(this->gameOverText);
      this->gameOverShown = true;
    }
    return;
  }
  if (std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::high_resolution_clock::now() - this->enemyMovementTimepoint).count() > this->timePerMovement) {
    this->enemyMovementTimepoint = std::chrono::high_resolution_clock::now();
    this->moveEnemies();
    unsigned int enemyStop = __MAX_SHOOTING_ENEMIES >= this->enemies.size()
      ? this->enemies.size()
      : __MAX_SHOOTING_ENEMIES;
    for (unsigned int idx = 0; idx < enemyStop; idx++) {
      this->makeEnemyShoot(this->getRandomEnemy());
    }
  }
  //this->makeEnemyShoot(this->getRandomEnemy());
}

void GameController::end() {

}

unsigned int GameController::getRandomEnemy() {
  if (enemies.empty()) {
    throw std::out_of_range("The enemies map is empty.");
  }
  std::vector<unsigned int> indexes;
  indexes.reserve(this->enemies.size());
  for (std::pair<unsigned int, std::shared_ptr<Object>> numberAndEnemy : this->enemies) {
    indexes.push_back(numberAndEnemy.first);
  }
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<unsigned int> dist(0, indexes.size());
  unsigned int generated = dist(rng);
  while(this->enemies.find(indexes[generated]) == this->enemies.end()) {
    generated = dist(rng);
  }
  return indexes[generated];
}

void GameController::triggerGameOver(bool isVictory) {
  this->gameOver = true;
  if (isVictory) {
    this->gameOverText->setText("You Win!");
  }
}

void GameController::makeBackground() {
  std::vector<glm::vec3> vertexes;
  std::vector<glm::vec4> colors;
	vertexes.push_back(glm::vec3(-1.0f,-1.0f, 0.0f));
	vertexes.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
	vertexes.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
	vertexes.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
  glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  colors.push_back(color);
  colors.push_back(color);
  colors.push_back(color);
  colors.push_back(color);
  int w = 1280;
  int h = 720;
  this->background =
    std::make_shared<Object>(GL_TRIANGLE_STRIP,
      this->sceneReference->getMainShader()->getId(), vertexes, colors);
  this->background->getScale().x = w;
  this->background->getScale().y = h;
  this->background->getPosition().x = (float)w / 2.0f;
  this->background->getPosition().y = (float)h / 2.0f;
  this->backgroundSetter = std::make_unique<Shader::UniformVariable>(
    this->sceneReference->getMainShader()->queryUniform("isBackground"));
  
  this->bgUniformUpdaterId = this->background->bindUniformUpdater(this->enableBGMode);
  this->sceneReference->addObject(background);
}

std::shared_ptr<Object> GameController::makeEnemy(glm::vec3 position) {
  std::vector<glm::vec3> enemyGeometry = enemyPrototype.geometry;
  std::vector<glm::vec4> enemyColors = enemyPrototype.colors;
  std::shared_ptr<Object> enemy = std::make_shared<Object>(GL_TRIANGLE_FAN,
    this->sceneReference->getMainShader()->getId(), enemyGeometry, enemyColors);
  enemy->getScale().x = 100;
  enemy->getScale().y = 100;
  enemy->setPosition(position);
  enemy->getRotation().z = 180;
  enemy->setName(std::string("enemy"));
  this->listenerIdentifiers.push_back(enemy->bindUniformUpdater(this->disableBGMode));
  this->enemies.emplace(this->enemies.size(), enemy);
  return enemy;
}

void GameController::makeEnemies() {
  this->enemyStep = DEFAULT_WINDOW_HEIGHT / (float) enemyNumber;
  this->initialEnemyX = (DEFAULT_WINDOW_WIDTH - (this->enemyStep * enemyNumber)) - 250.0f;
  float enemyX = initialEnemyX;
  float enemyY = DEFAULT_WINDOW_HEIGHT - 50.0f;
  for (unsigned int level = 0; level < enemyLevels; level ++) {
    for (unsigned int enemies = 0; enemies < enemyNumber; enemies ++) {
      std::shared_ptr<Object> enemy = this->makeEnemy(glm::vec3(enemyX, enemyY, 1.0f));
      this->sceneReference->addObject(enemy);
      this->sceneReference->attachBehaviour(enemy,
        std::make_shared<RectangularCollider>(
          glm::vec3(-0.38f, -0.23f, 0.0f), glm::vec3(0.42f, 0.6f, 0.0f)));
      enemyX += this->enemyStep;
    }
    enemyY -= 100.0f;
    enemyX = initialEnemyX;
  }
}

void GameController::makePlayer(glm::vec3 position) {
  std::vector<glm::vec3> playerGeometry = playerPrototype.geometry;
  std::vector<glm::vec4> playerColors = playerPrototype.colors;
  std::shared_ptr<Object> player = std::make_shared<Object>(GL_TRIANGLE_FAN,
    this->sceneReference->getMainShader()->getId(), playerGeometry, playerColors);
  player->getScale().x = 100;
  player->getScale().y = 100;
  player->setPosition(position);
  player->setName(std::string("player"));
  this->listenerIdentifiers.push_back(player->bindUniformUpdater(this->disableBGMode));
  this->player = player;
  this->sceneReference->addObject(this->player);
  this->sceneReference->attachBehaviour(this->player, std::make_shared<CharacterController>(this->player, this->sceneReference));
  std::shared_ptr<RectangularCollider> rectangularCollider = std::make_shared<RectangularCollider>(
    glm::vec3(0.38f, 0.23f, 0.0f), glm::vec3(-0.38f, -0.23f, 0.0f));
  this->sceneReference->attachBehaviour(this->player, rectangularCollider);
}

void GameController::makeMiscData() {
  HermiteCurve definedGeometry("proiettile.herm");
  definedGeometry.calculate();
  this->projectilePrototype.geometry = definedGeometry.getCurvePoints(glm::vec3(0.0f, 0.0f, 0.0f));
  std::vector<glm::vec4> colors;
  for (glm::vec3 vertex : this->projectilePrototype.geometry) {
    colors.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  }
  this->projectilePrototype.colors = std::move(colors);
  this->gameOverText = std::make_shared<Label>("Game Over!", glm::vec3(1.0f, 0.0f, 0.0f));
  this->gameOverText->setPosition(glm::vec3(DEFAULT_WINDOW_WIDTH / 2.0f,
    DEFAULT_WINDOW_HEIGHT / 2.0f, 1.0f));
}

void GameController::makeEnemyData() {
  HermiteCurve definedGeometry("sottomarino.herm");
  definedGeometry.calculate();
  this->enemyPrototype.geometry = definedGeometry.getCurvePoints(glm::vec3(0.0f, 0.0f, 0.0f));
  std::vector<glm::vec4> colors;
  for (glm::vec3 vertex : this->enemyPrototype.geometry) {
    colors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  }
  this->enemyPrototype.colors = std::move(colors);
}

void GameController::makePlayerData() {
  HermiteCurve definedGeometry("navicella.herm");
  definedGeometry.calculate();
  this->playerPrototype.geometry = definedGeometry.getCurvePoints();
  std::vector<glm::vec4> colors;
  for (glm::vec3 vertex : this->enemyPrototype.geometry) {
    colors.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  }
  this->playerPrototype.colors = std::move(colors);
}

void GameController::makeText() {
  Label scoreLabel(std::string("Score: ") +
    std::to_string(FirstGameData::getInstance().getHealth()),
    glm::vec3(1.0f, 1.0f, 1.0f));
  scoreLabel.getPosition().x = 10.0f;
  scoreLabel.getPosition().y = 10.0f;
  scoreLabel.getPosition().z = 1.0f;
  scoreLabel.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
  scoreLabel.setVisible(true);
  Label healthLabel(std::string("Health: ") +
    std::to_string(FirstGameData::getInstance().getHealth()),
    glm::vec3(1.0f, 1.0f, 1.0f));
  healthLabel.getPosition().x = DEFAULT_WINDOW_WIDTH - 250.0f;
  healthLabel.getPosition().y = 10.0f;
  healthLabel.getPosition().z = 1.0f;
  healthLabel.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
  healthLabel.setVisible(true);
  this->score = std::make_shared<Label>(std::move(scoreLabel));
  this->health = std::make_shared<Label>(std::move(healthLabel));
  this->sceneReference->addText(this->score);
  this->sceneReference->addText(this->health);
}

void GameController::moveEnemies() {
  for (std::pair<unsigned int, std::shared_ptr<Object>> enemy : this->enemies) {
    glm::vec3 nextPosition = enemy.second->getPosition();
    nextPosition.x += this->enemyStep;
    if (nextPosition.x > DEFAULT_WINDOW_WIDTH) {
      nextPosition.x = 50.0f;
      nextPosition.y -= 100.0f;
    }
    if (nextPosition.y < (DEFAULT_WINDOW_HEIGHT * 0.45f)) {
      this->triggerGameOver(false);
    }
    enemy.second->setPosition(nextPosition);
  }
}

void GameController::makeEnemyShoot(unsigned int enemyNumber) {
  //printf("%i\n", enemyNumber);
  std::vector<glm::vec3> projectileGeometry = projectilePrototype.geometry;
  std::vector<glm::vec4> projectileColors = projectilePrototype.colors;
  std::shared_ptr<Object> projectile = std::make_shared<Object>(GL_TRIANGLE_FAN,
    this->sceneReference->getMainShader()->getId(), projectileGeometry, projectileColors);
  projectile->setScale(glm::vec3(10.0f, 10.0f, 10.0f));
  glm::vec3 positionCopy = this->enemies.at(enemyNumber)->getPosition();
  projectile->setPosition(positionCopy);
  projectile->setName("projectile");
  this->sceneReference->addObject(projectile);
  std::shared_ptr<RectangularCollider> rectangularCollider =
    std::make_shared<RectangularCollider>(glm::vec3(1.0f, 1.0f, 0.0f),
      glm::vec3(-1.0f, -1.0f, 0.0f));
  rectangularCollider->registerCollisionListener(this->enemyBulletFunction);
  this->sceneReference->attachBehaviour(
    projectile, std::make_shared<Projectile>(projectile, this->sceneReference, -1000.0f));
  this->sceneReference->attachBehaviour(
    projectile, rectangularCollider);
}
