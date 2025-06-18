#include "Game.h"
#include "combat.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <thread>
#include <chrono>

Game::Game() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    createWorld();
}

void Game::showStatus() const {
    SetColor(11);
    std::cout << "==== PLAYER ====" << "  HP: " << player.health << "/" << player.maxHealth
              << "  MP: " << player.mana << "/" << player.maxMana
              << "  ATK: " << player.attackPower << "  DEF: " << player.defense
              << "  LVL: " << player.level << "\n";
    SetColor(7);
}

void Game::showActions() {
    std::cout << "\nДоступные действия:\n";
    SetColor(10);
    std::cout << "1. Осмотреться\n2. Взять предмет\n3. Идти\n4. Инвентарь\n5. Использовать предмет\n6. Сохранить игру\n7. Помощь\n8. Выйти\n";
    SetColor(7);
}

void Game::displayCurrentState() const {
    if (!currentRoom) {
        std::cout << "[ERROR] currentRoom is nullptr!\n";
        std::cout.flush();
        return;
    }

    std::ostream::sync_with_stdio(true);

    std::cout.flush();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    system("cls");

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    
    showStatus();
    std::cout.flush();
    
    std::cout << "\n" << currentRoom->description << "\n";
    std::cout.flush();
    
    if (!currentRoom->items.empty()) {
        SetColor(14);
        std::cout << "Вы видите следующие предметы:\n";
        for (const auto& item : currentRoom->items) {
            std::cout << "- " << item.name << ": " << item.description << "\n";
        }
        SetColor(7);
        std::cout.flush();
    }

    std::cout.flush();
}

void Game::showHelp() {
    std::cout << "\nКоманды:\n"
                 "1. Осмотреться  - повторно вывести описание комнаты\n"
                 "2. Взять предмет - выбрать и добавить предмет в инвентарь\n"
                 "3. Идти         - список направлений\n"
                 "4. Инвентарь    - посмотреть содержимое\n"
                 "5. Использовать - применить предмет\n"
                 "6. Сохранить    - записать прогресс (save.txt)\n"
                 "7. Помощь       - вывести эту справку\n"
                 "8. Выход        - завершить игру\n";
}

void Game::saveGame() const {
    std::ofstream out("save.txt");
    if(!out) return;
    size_t roomIndex = 0;
    for(size_t i=0; i < allRooms.size(); ++i) { if(allRooms[i] == currentRoom) { roomIndex = i; break; } }
    out << roomIndex << " " << portalOpened << " " << chestOpened << "\n";
    out << player.health << " " << player.maxHealth << " " << player.mana << " " << player.maxMana << " "
        << player.attackPower << " " << player.defense << " " << player.level << " " << player.experience << " "
        << player.weaponEquipped << " " << player.shieldEquipped << "\n";
    out << player.inventory.size() << "\n";
    for(const auto& it : player.inventory) {
        out << it.name << "|" << it.description << "|" << it.type << "|" << it.power << "\n";
    }
    std::cout << "Игра сохранена.\n";
}

bool Game::loadGame() {
    std::ifstream in("save.txt");
    if(!in) return false;
    size_t roomIndex; in >> roomIndex >> portalOpened >> chestOpened;
    in >> player.health >> player.maxHealth >> player.mana >> player.maxMana >> player.attackPower >> player.defense >> player.level >> player.experience >> player.weaponEquipped >> player.shieldEquipped;
    size_t invSize; in >> invSize;
    in.ignore();
    player.inventory.clear();
    for(size_t i=0; i < invSize; ++i) {
        std::string line; std::getline(in, line);
        size_t p1 = line.find('|'); size_t p2 = line.find('|', p1 + 1); size_t p3 = line.find('|', p2 + 1);
        if(p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos) continue;
        std::string n = line.substr(0, p1);
        std::string d = line.substr(p1 + 1, p2 - p1 - 1);
        std::string t = line.substr(p2 + 1, p3 - p2 - 1);
        int pow = std::stoi(line.substr(p3 + 1));
        player.inventory.emplace_back(n, d, t, pow);
    }
    if(roomIndex < allRooms.size()) currentRoom = allRooms[roomIndex];
    std::cout << "Сохранение загружено.\n";
    return true;
}

void Game::run() {
    bool newGame = false;
    if (loadGame()) {
        std::cout << "Хотите продолжить сохранённую игру? (y/n): ";
        char c = getCharInput();
        if (c == 'n' || c == 'N') {
            newGame = true;
        }
    } else {
        newGame = true;
    }

    if (newGame) {
        createWorld();
    }

    displayCurrentState();
    showActions();
    
    while (true) {
        if (currentRoom == portalHall && !portalOpened && player.hasItem("Рунический Ключ")) {
            std::cout << "\nВы сняли магическую печать с портала!\n";
            portalOpened = true;
            displayCurrentState();
            showActions();
        }
        if (currentRoom == chestRoom && !chestOpened && player.hasItem("Скелетный Ключ")) {
            std::cout << "\nВы открыли сундук с помощью Скелетного Ключа!\n";
            chestOpened = true;
            displayCurrentState();
            showActions();
        }
        if (currentRoom->enemy) {
            std::cout << "\n";
            combat(player, currentRoom->enemy.get());
            currentRoom->enemy.reset();
            if (player.health <= 0) {
                std::cout << "Вы были повержены.\n";
                break;
            }
            if (currentRoom == throneRoom) {
                 if(player.hasItem("Артефакт Света")){
                    SetColor(10);
                    std::cout << "Артефакт Света уничтожает Лорда Некроманта! Вы победили!\n"; SetColor(7);
                } else {
                    SetColor(12);
                    std::cout << "Без Артефакта Света победить Лорда Некроманта невозможно...\n"; SetColor(7);
                }
                break;
            }
            if (player.hasItem("Золотой Кубок") && currentRoom->description.find("дракон")!=std::string::npos){
                SetColor(10);
                std::cout << "Вы победили и выбрались из подземелья с Золотым Кубком!\n";
                break;
            }
            pauseConsole();
            displayCurrentState();
            showActions();
            continue;
        }

        std::cout << "Выберите действие: ";
        std::cout.flush();
        int action = getIntInput(1, 8);
        
        switch (action) {
            case 1:
                displayCurrentState();
                pauseConsole();
                displayCurrentState();
                showActions();
                continue;
            case 2: {
                if (currentRoom->items.empty()) {
                    std::cout << "\nЗдесь нет предметов.\n";
                } else {
                    std::cout << "\nДоступные предметы:\n";
                    for (size_t i = 0; i < currentRoom->items.size(); ++i) {
                        std::cout << i + 1 << ". " << currentRoom->items[i].name << "\n";
                    }
                    std::cout << "Ваш выбор (0 для отмены): ";
                    int idx = getIntInput(0, currentRoom->items.size());
                    if (idx > 0) {
                        Item takenItem = currentRoom->items[idx - 1];
                        player.addItem(takenItem);
                        currentRoom->items.erase(currentRoom->items.begin() + idx - 1);
                        std::cout << "Вы взяли: " << takenItem.name << "\n";
                    }
                }
                pauseConsole();
                break;
            }
            case 3: {
                auto dirs = currentRoom->getExitDirections();
                if (dirs.empty()) {
                    std::cout << "\nНет выходов.\n";
                    pauseConsole();
                    break;
                }
                std::cout << "\nДоступные направления:\n";
                for (size_t i = 0; i < dirs.size(); ++i) {
                    std::cout << i + 1 << ". " << dirs[i] << "\n";
                }
                std::cout << "Выберите направление (0 для отмены): ";
                int d = getIntInput(0, dirs.size());
                if (d > 0) {
                    if (currentRoom == portalHall && !portalOpened && d <= static_cast<int>(dirs.size()) && dirs[d - 1] == "тронный зал") {
                        std::cout << "\nПортал закрыт магической печатью.\n";
                        pauseConsole();
                        break;
                    }
                    auto next = currentRoom->getExitByIndex(d);
                    if (next) {
                        currentRoom = next;
                        std::cout << "\nВы перешли в новую область.\n";
                        pauseConsole();
                        displayCurrentState();
                        showActions();
                        continue;
                    } else {
                        std::cout << "\nВы не можете пройти туда.\n";
                        pauseConsole();
                    }
                } else {
                    std::cout << "\nВы остались на месте.\n";
                    pauseConsole();
                }
                break;
            }
            case 4:
                std::cout << "\n";
                player.showInventory();
                pauseConsole();
                break;
            case 5:
                std::cout << "\n";
                player.useItem();
                pauseConsole();
                break;
            case 6:
                std::cout << "\n";
                saveGame();
                pauseConsole();
                break;
            case 7:
                std::cout << "\n";
                showHelp();
                pauseConsole();
                break;
            case 8:
                std::cout << "\nСпасибо за игру!\n";
                return;
            default:
                std::cout << "\nНеверный выбор. Повторите попытку.\n";
                pauseConsole();
                break;
        }
        
        displayCurrentState();
        showActions();
    }
}

void Game::createWorld() {
    auto room1 = std::make_shared<Room>("Вы находитесь в тёмной комнате. Единственный выход ведёт на север.");
    auto room2 = std::make_shared<Room>("Вы в освещённом коридоре. Есть выходы на юг, восток и запад.");
    auto room3 = std::make_shared<Room>("Вы нашли тайную комнату с сокровищем!");
    auto room4 = std::make_shared<Room>("Вы в большой зале, где вас поджидает опасный враг.");
    auto room5 = std::make_shared<Room>("Вы в библиотеке, окружены старыми книгами.");
    auto room6 = std::make_shared<Room>("Вы в заброшенной оружейной, вокруг разбросано старое оружие.");
    auto room7 = std::make_shared<Room>("Вы подошли к выходу из подземелья, но путь преграждает могучий дракон!");

    auto room8  = std::make_shared<Room>("Вы в подземном лесу. Здесь пахнет сыростью и слышны странные звуки.");
    portalHall   = std::make_shared<Room>("Вы в зале с магическим порталом. Портал закрыт магической печатью.");
    auto room9  = std::make_shared<Room>("Вы в древнем храме, стены покрыты рунами.");
    auto room10 = std::make_shared<Room>("Вы на берегу подземного озера. Вода светится голубым светом.");
    auto room11 = std::make_shared<Room>("Вы в заброшенной лаборатории алхимика. Здесь много странных колб и книг.");
    auto room12 = std::make_shared<Room>("Вы в мрачной тюрьме. В углу лежит скелет с ключом.");
    chestRoom   = std::make_shared<Room>("Вы в тайной комнате с сундуком. Сундук закрыт на замок.");
    throneRoom  = std::make_shared<Room>("Вы в тронном зале, где сидит Лорд Некромант!");

    room1->setExit("север", room2);
    room2->setExit("юг", room1);
    room2->setExit("восток", room3);
    room2->setExit("запад", room5);
    room2->setExit("север", room4);
    room2->setExit("лес", room8);

    room3->setExit("запад", room2);

    room5->setExit("восток", room2);
    room5->setExit("север", room6);

    room6->setExit("юг", room5);
    room6->setExit("север", room7);

    room7->setExit("юг", room6);

    room4->setExit("юг", room2);

    room8->setExit("коридор", room2);
    room8->setExit("озеро", room10);
    room8->setExit("храм", room9);

    room9->setExit("лес", room8);
    room9->setExit("лаборатория", room11);

    room11->setExit("храм", room9);
    room11->setExit("тюрьма", room12);

    room12->setExit("лаборатория", room11);
    room12->setExit("зал портала", portalHall);

    portalHall->setExit("тюрьма", room12);
    portalHall->setExit("тайная комната", chestRoom);
    portalHall->setExit("тронный зал", throneRoom);

    chestRoom->setExit("зал портала", portalHall);

    throneRoom->setExit("зал портала", portalHall);

    room10->setExit("лес", room8);
    room3->addItem(Item("Золотой Кубок", "Древний артефакт неизмеримой ценности.", "Квестовый предмет", 0));
    room5->addItem(Item("Зелье Здоровья", "Восстанавливает 50 HP.", "Зелье", 50));
    room5->addItem(Item("Эликсир Маны", "Восстанавливает 30 MP.", "Мана", 30));
    room6->addItem(Item("Меч Воина", "Увеличивает силу атаки на 10.", "Оружие", 10));
    room6->addItem(Item("Щит Защитника", "Уменьшает входящий урон.", "Щит", 5));
    room8->addItem(Item("Гриб Светляк", "Восстанавливает 20 HP.", "Зелье", 20));
    room9->addItem(Item("Рунический Ключ", "Может открыть магическую печать.", "Ключ", 0));
    room11->addItem(Item("Зелье Силы", "Временно увеличивает атаку на 15.", "Зелье Силы", 15));
    room12->addItem(Item("Скелетный Ключ", "Открывает сундук в тайной комнате.", "Ключ", 0));
    chestRoom->addItem(Item("Артефакт Света", "Необходим для победы над Лордом.", "Квестовый предмет", 0));
    room10->addItem(Item("Вода Жизни", "Восстанавливает всё здоровье.", "Зелье", 999));
    room10->addItem(Item("Бомба", "Наносит 30 урона врагу.", "Бомба", 30));
    room10->addItem(Item("Большой Эликсир Маны", "Восстанавливает 50 MP.", "Мана", 50));

    room4->setEnemy(std::make_unique<Enemy>("Гоблин", 50, 8, "Яд", 2));
    room7->setEnemy(std::make_unique<Enemy>("Дракон", 150, 14, "Огненное дыхание", 1));
    room8->setEnemy(std::make_unique<Enemy>("Гигантский Паук", 60, 10, "Яд", 2));
    room9->setEnemy(std::make_unique<Enemy>("Страж Храма", 80, 12, "Проклятие", 1));
    room11->setEnemy(std::make_unique<Enemy>("Безумный Алхимик", 70, 10, "Огненное дыхание", 1));
    room12->setEnemy(std::make_unique<Enemy>("Тюремный Призрак", 50, 8, "Проклятие", 2));
    portalHall->setEnemy(std::make_unique<Enemy>("Портальный Голем", 100, 14, "Огненное дыхание", 1));
    throneRoom->setEnemy(std::make_unique<Enemy>("Лорд Некромант", 100, 12, "Проклятие", 1));
    allRooms = {room1,room2,room3,room4,room5,room6,room7,room8,room9,room10,room11,room12,portalHall,chestRoom,throneRoom};
    currentRoom = room1;
} 