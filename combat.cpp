#include "combat.h"
#include "utils.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>

void combat(Player& player, Enemy* enemy) {
    SetColor(12);
    std::cout << "Вы столкнулись с " << enemy->name << "!\n";
    SetColor(7);

    int poisonTurns = 0;
    int burnTurns = 0;
    bool cursed = false;
    bool blocked = false;

    while (player.health > 0 && enemy->health > 0) {
        if(poisonTurns > 0){
            int pdmg = 5;
            player.health -= pdmg;
            SetColor(12);
            std::cout << "Яд наносит вам " << pdmg << " урона. Осталось ходов: " << poisonTurns-1 << "\n";
            SetColor(7);
            poisonTurns--; if(player.health<=0) break;
        }
        if(burnTurns > 0){
            int fdmg = 7;
            player.health -= fdmg;
            SetColor(12);
            std::cout << "Ожоги наносят вам " << fdmg << " урона. Осталось ходов: " << burnTurns-1 << "\n";
            SetColor(7);
            burnTurns--; if(player.health<=0) break;
        }

        SetColor(11);
        std::cout << "\nВаше здоровье: " << player.health << "/" << player.maxHealth
                  << " | Мана: " << player.mana << "/" << player.maxMana << "\n";
        std::cout << "Здоровье врага: " << enemy->health << "\n";
        SetColor(7);
        SetColor(10);
        std::cout << "1. Атаковать\n2. Использовать предмет\n3. Использовать специальный навык\n4. Блокировать\n5. Бежать\nВыберите действие: ";
        SetColor(7);
        int choice = getIntInput(1, 5);

        if (choice == 1) {
            bool critical = (std::rand() % 100) < 10;
            int effectiveAttack = player.getEffectiveAttack();
            int damage = effectiveAttack + (critical ? effectiveAttack / 2 : 0);
            enemy->health -= damage;
            if (critical) {
                SetColor(14);
                std::cout << "Критический удар! Вы нанесли " << damage << " урона врагу.\n";
                SetColor(7);
            }
            else {
                SetColor(6);
                std::cout << "Вы нанесли " << damage << " урона врагу.\n";
                SetColor(7);
            }

            if (enemy->health <= 0) {
                SetColor(10);
                std::cout << "Вы победили " << enemy->name << "!\n";
                SetColor(7);
                player.gainExperience(20);
                return;
            }

            if (!enemy->specialAbility.empty()) {
            }

            if (player.health <= 0) {
                SetColor(12);
                std::cout << "Вы были повержены...\n";
                SetColor(7);
                exit(0);
            }
        }
        else if (choice == 2) {
            player.useItem(enemy);
            if(enemy->health <=0){
                SetColor(10);
                std::cout << "Враг повержен!\n";
                SetColor(7);
                player.gainExperience(20);
                return;
            }
        }
        else if (choice == 3) {
            SetColor(13);
            player.useSpecialSkill(*enemy);
            SetColor(7);
            if (enemy->health <= 0) {
                SetColor(10);
                std::cout << "Вы победили " << enemy->name << "!\n";
                SetColor(7);
                player.gainExperience(20);
                return;
            }
        }
        else if (choice == 4) {
            SetColor(9);
            std::cout << "Вы блокируете атаку врага, снижая получаемый урон на 50%.\n";
            blocked = true;
        }
        else if (choice == 5) {
            SetColor(10);
            std::cout << "Вы успешно сбежали!\n";
            SetColor(7);
            return;
        }

        for(int s=0; s<enemy->speed && player.health >0 && enemy->health>0; ++s){
            int dmg = std::max(1, enemy->attackPower - player.defense);
            if(blocked){ dmg /=2; blocked=false; }
            SetColor(12);
            player.health -= dmg;
            std::cout << enemy->name << " нанёс вам " << dmg << " урона.\n";
            if(player.health<=0){
                SetColor(12);
                std::cout << "Вы были повержены...\n";
                SetColor(7);
                exit(0);
            }
        }

        if(!enemy->specialAbility.empty() && (std::rand()%100)<30){
            enemy->useSpecialAbility();
            if(enemy->specialAbility=="Яд") poisonTurns=3;
            else if(enemy->specialAbility=="Огненное дыхание") burnTurns=2;
            else if(enemy->specialAbility=="Проклятие" && !cursed){
                cursed=true; player.attackPower = std::max(1, player.attackPower-3);
                SetColor(5);
                std::cout << "Ваши силы ослабели под действием проклятия!\n";
                SetColor(7);
            }
        }

        player.decrementPotionEffects();
    }
} 