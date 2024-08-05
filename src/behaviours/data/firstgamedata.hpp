#pragma once

class FirstGameData {
  private:
    unsigned int score;
    unsigned int health;

    FirstGameData();
    FirstGameData(const FirstGameData&) = delete;
    FirstGameData& operator=(const FirstGameData&) = delete;
    ~FirstGameData() = default;
  public:
    static FirstGameData& getInstance();
    void setScore(unsigned int score);
    unsigned int getScore();
    void setHealth(unsigned int health);
    unsigned int getHealth();
};
