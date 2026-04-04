#pragma once

#include <string>

class Item
{
    std::string name;

public:
    Item(std::string name) : name(name) {}

    std::string getName() const
    {
        return name;
    }
};
