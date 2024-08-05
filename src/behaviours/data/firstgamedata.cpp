#include "firstgamedata.hpp"

FirstGameData::FirstGameData() {
  this->health = 0;
  this->score = 0;
}

FirstGameData& FirstGameData::getInstance() {
  static FirstGameData firstGameData;
  return firstGameData;
}

void FirstGameData::setHealth(unsigned int health) {
  this->health = health;
}
unsigned int FirstGameData::getHealth() {
  return this->health;
}

void FirstGameData::setScore(unsigned int score) {
  this->score = score;
}
unsigned int FirstGameData::getScore() {
  return this->score;
}
