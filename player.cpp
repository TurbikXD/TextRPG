#include "player.h"
#include <iostream>
#include <limits>
#include "utils.h"
#include <algorithm>

Player::Player() : health(100), maxHealth(100), attackPower(10), defense(0), level(1), experience(0), mana(50), maxMana(50), weaponEquipped(false), shieldEquipped(false), strengthPotionTurns(0), strengthPotionBonus(0) {}

void Player::addItem(const Item& item) {
    SetColor(10);
    inventory.push_back(item);
    std::cout << "Вы получили предмет: " << item.name << " - " << item.description << "\n";
    SetColor(7);
}

void Player::showInventory() const {
    if (inventory.empty() && !weaponEquipped && !shieldEquipped) {
        SetColor(12);
        std::cout << "Ваш инвентарь пуст.\n";
        SetColor(7);
        return;
    }
    
    SetColor(14);
    std::cout << "Ваш инвентарь:\n";
    for (size_t i = 0; i < inventory.size(); ++i) {
        std::cout << i + 1 << ". " << inventory[i].name << " (" << inventory[i].type << "): "
                  << inventory[i].description << "\n";
    }
    
    if (weaponEquipped || shieldEquipped) {
        SetColor(10);
        std::cout << "\nЭкипировано:\n";
        if (weaponEquipped) {
            std::cout << "• Оружие экипировано\n";
        }
        if (shieldEquipped) {
            std::cout << "• Щит экипирован\n";
        }
    }
    
    SetColor(7);
}

void Player::useItem() {
    useItem(nullptr);
}

void Player::useItem(Enemy* enemy) {
    if (inventory.empty()) {
        SetColor(12);
        std::cout << "Ваш инвентарь пуст.\n";
        SetColor(7);
        return;
    }
    showInventory();
    std::cout << "Выберите номер предмета для использования (0 для отмены): ";
    int choice = getIntInput(0, inventory.size());
    
    if (choice == 0) {
        std::cout << "Отменено.\n";
        return;
    }
    
    Item selectedItem = inventory[choice - 1];

    if (selectedItem.type == "Зелье") {
        if (health >= maxHealth) {
            SetColor(12);
            std::cout << "Ваше здоровье уже максимально.\n";
            SetColor(7);
            return;
        }
        health += selectedItem.power;
        if (health > maxHealth) health = maxHealth;
        SetColor(10);
        std::cout << "Вы использовали " << selectedItem.name << " и восстановили здоровье до " << health << ".\n";
        SetColor(7);
        inventory.erase(inventory.begin() + choice - 1);
    }
    else if (selectedItem.type == "Зелье Силы") {
        strengthPotionTurns = 5;
        strengthPotionBonus = selectedItem.power;
        SetColor(10);
        std::cout << "Вы использовали " << selectedItem.name << ".\n";
        std::cout << "Ваша атака временно увеличена на " << strengthPotionBonus << " на " << strengthPotionTurns << " ходов!\n";
        SetColor(7);
        inventory.erase(inventory.begin() + choice - 1);
    }
    else if (selectedItem.type == "Оружие") {
        if (weaponEquipped) {
            SetColor(12);
            std::cout << "Вы уже экипировали оружие. Сначала снимите текущее.\n";
            SetColor(7);
            return;
        }
        attackPower += selectedItem.power;
        weaponEquipped = true;
        SetColor(10);
        std::cout << "Вы экипировали " << selectedItem.name << ". Сила атаки увеличена до " << attackPower << ".\n";
        SetColor(7);
        inventory.erase(inventory.begin() + choice - 1);
    }
    else if (selectedItem.type == "Щит") {
        if (shieldEquipped) {
            SetColor(12);
            std::cout << "Вы уже экипировали щит. Сначала снимите текущий.\n";
            SetColor(7);
            return;
        }
        defense += selectedItem.power;
        shieldEquipped = true;
        SetColor(10);
        std::cout << "Вы экипировали " << selectedItem.name << ". Защита увеличена до " << defense << ".\n";
        SetColor(7);
        inventory.erase(inventory.begin() + choice - 1);
    }
    else if (selectedItem.type == "Мана") {
        if (mana >= maxMana) {
            SetColor(12);
            std::cout << "Ваша мана уже максимальна.\n";
            SetColor(7);
            return;
        }
        mana += selectedItem.power;
        if (mana > maxMana) mana = maxMana;
        SetColor(10);
        std::cout << "Вы использовали " << selectedItem.name << " и восстановили ману до " << mana << ".\n";
        SetColor(7);
        inventory.erase(inventory.begin() + choice - 1);
    }
    else if (selectedItem.type == "Бомба") {
        if (enemy != nullptr) {
            int dmg = selectedItem.power;
            enemy->health -= dmg;
            SetColor(14);
            std::cout << "Вы бросили бомбу и нанесли " << dmg << " урона врагу!\n";
            SetColor(7);
            inventory.erase(inventory.begin() + choice - 1);
        } else {
            SetColor(12);
            std::cout << "Бомбу можно использовать только в бою!\n";
            SetColor(7);
        }
    }
    else if (selectedItem.type == "Ключ") {
        SetColor(14);
        std::cout << "Этот предмет используется автоматически при необходимости.\n";
        SetColor(7);
    }
    else if (selectedItem.type == "Квестовый предмет") {
        if (selectedItem.name == "Артефакт Света" && enemy != nullptr) {
            int damage = 80;
            enemy->health -= damage;
            SetColor(14);
            std::cout << "Артефакт Света излучает ослепительный свет и наносит " << damage << " урона врагу!\n";
            SetColor(7);
            inventory.erase(inventory.begin() + choice - 1);
        } else {
            SetColor(14);
            std::cout << "Этот предмет используется автоматически при необходимости.\n";
            SetColor(7);
        }
    }
    else {
        SetColor(12);
        std::cout << "Этот предмет нельзя использовать.\n";
        SetColor(7);
    }
}

void Player::useSpecialSkill(Enemy& enemy) {
    if (mana >= 20) {
        mana -= 20;
        int damage = getEffectiveAttack() * 2;
        SetColor(13);
        enemy.health -= damage;
        std::cout << "Вы используете специальный навык и наносите " << damage << " урона!\n";
        SetColor(7);
        gainExperience(10);
    }
    else {
        SetColor(12);
        std::cout << "Недостаточно маны для использования навыка.\n";
        SetColor(7);
    }
}

void Player::gainExperience(int exp) {
    experience += exp;
    SetColor(10);
    std::cout << "Вы получили " << exp << " опыта. Общий опыт: " << experience << ".\n";
    SetColor(7);
    if (experience >= level * 100) {
        levelUp();
    }
}

void Player::levelUp() {
    level++;
    maxHealth += 20;
    health = maxHealth;
    maxMana += 10;
    mana = maxMana;
    attackPower += 5;
    SetColor(11);
    std::cout << "Поздравляем! Вы достигли уровня " << level << "!\n";
    std::cout << "Ваше здоровье увеличено до " << maxHealth << ", мана до " << maxMana << ", сила атаки до " << attackPower << ".\n";
    SetColor(7);
}

bool Player::hasItem(const std::string& itemName) const {
    return std::any_of(inventory.begin(), inventory.end(), [&itemName](const Item& i) {
        return i.name == itemName;
    });
}

int Player::getEffectiveAttack() const {
    int totalAttack = attackPower;
    if (strengthPotionTurns > 0) {
        totalAttack += strengthPotionBonus;
    }
    return totalAttack;
}

void Player::decrementPotionEffects() {
    if (strengthPotionTurns > 0) {
        strengthPotionTurns--;
        if (strengthPotionTurns == 0) {
            SetColor(12);
            std::cout << "Эффект зелья силы закончился.\n";
            SetColor(7);
        }
    }
} 