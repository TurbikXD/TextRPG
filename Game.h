#pragma once
#include <memory>
#include <vector>
#include "player.h"
#include "room.h"

class Game {
public:
    Game();
    void run();
private:
    Player player;
    std::shared_ptr<Room> currentRoom;

    bool portalOpened{false};
    bool chestOpened{false};

    std::shared_ptr<Room> portalHall;
    std::shared_ptr<Room> chestRoom;
    std::shared_ptr<Room> throneRoom;

    void displayCurrentState() const;
    void createWorld();
    void showStatus() const;
    static void showActions() ;
    static void showHelp() ;
    void saveGame() const;
    bool loadGame();

    std::vector<std::shared_ptr<Room>> allRooms;
}; 