#pragma once
#include <string>

class Enemy {
public:
    std::string name;
    int health;
    int attackPower;
    int speed;
    std::string specialAbility;

    Enemy(std::string n, int h, int a, std::string sa, int spd = 1);
    Enemy(std::string n, int h, int a, std::string sa);
    void useSpecialAbility() const;
}; 