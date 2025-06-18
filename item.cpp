#include "item.h"

Item::Item(std::string n, std::string d, std::string t, int p)
    : name(std::move(n)), description(std::move(d)), type(std::move(t)), power(p) {} 