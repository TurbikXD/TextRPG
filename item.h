#pragma once
#include <string>

class Item {
public:
    std::string name;
    std::string description;
    std::string type;
    int power;

    Item(std::string n, std::string d, std::string t, int p);
    Item() = default;
}; 