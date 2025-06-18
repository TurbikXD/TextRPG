#pragma once
#include <vector>
#include <string>
#include "item.h"
#include "enemy.h"

class Player {
public:
    int health;
    int maxHealth;
    int attackPower;
    int defense;
    int level;
    int experience;
    int mana;
    int maxMana;
    std::vector<Item> inventory;

    bool weaponEquipped;
    bool shieldEquipped;

    int strengthPotionTurns;
    int strengthPotionBonus;

    Player();
    void addItem(const Item& item);
    void showInventory() const;
    void useItem();
    void useItem(Enemy* enemy);
    void useSpecialSkill(Enemy& enemy);
    void gainExperience(int exp);
    [[nodiscard]] bool hasItem(const std::string& itemName) const;
    [[nodiscard]] int getEffectiveAttack() const;
    void decrementPotionEffects();
private:
    void levelUp();
}; 