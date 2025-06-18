#include "room.h"
#include <utility>

Room::Room(std::string desc) : description(std::move(desc)), enemy(nullptr) {}

void Room::setExit(const std::string& direction, std::shared_ptr<Room> room) {
    for (auto& p : exits) {
        if (p.first == direction) {
            p.second = std::move(room);
            return;
        }
    }
    exits.emplace_back(direction, std::move(room));
}

void Room::addItem(const Item& item) {
    items.push_back(item);
}

void Room::setEnemy(std::unique_ptr<Enemy> e) {
    enemy = std::move(e);
}

std::shared_ptr<Room> Room::getExitByIndex(int index) const {
    if (index < 1 || index > static_cast<int>(exits.size()))
        return nullptr;
    return exits[index - 1].second;
}

std::vector<std::string> Room::getExitDirections() const {
    std::vector<std::string> directions;
    directions.reserve(exits.size());
    for (const auto& exit : exits) {
        directions.push_back(exit.first);
    }
    return directions;
} 