#pragma once
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include "item.h"
#include "enemy.h"

class Room {
public:
    std::string description;
    std::vector<std::pair<std::string, std::shared_ptr<Room>>> exits;
    std::vector<Item> items;
    std::unique_ptr<Enemy> enemy;

    explicit Room(std::string desc);
    void setExit(const std::string& direction, std::shared_ptr<Room> room);
    void addItem(const Item& item);
    void setEnemy(std::unique_ptr<Enemy> e);
    [[nodiscard]] std::shared_ptr<Room> getExitByIndex(int index) const;
    [[nodiscard]] std::vector<std::string> getExitDirections() const;
}; 