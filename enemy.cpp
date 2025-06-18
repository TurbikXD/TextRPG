#include "enemy.h"
#include <iostream>
#include <windows.h>

Enemy::Enemy(std::string n, int h, int a, std::string sa)
    : name(std::move(n)), health(h), attackPower(a), speed(1), specialAbility(std::move(sa)) {}

Enemy::Enemy(std::string n, int h, int a, std::string sa, int spd)
    : name(std::move(n)), health(h), attackPower(a), speed(spd), specialAbility(std::move(sa)) {}

void Enemy::useSpecialAbility() const {
    if (specialAbility == "Яд") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        std::cout << name << " использует ядовитую атаку!\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
    else if (specialAbility == "Огненное дыхание") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        std::cout << name << " выпускает струю огня!\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
    else if (specialAbility == "Проклятие") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
        std::cout << name << " накладывает проклятие!\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
} 