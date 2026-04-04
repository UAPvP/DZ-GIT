#pragma once

#include <string>

#include "ItemSlot.h"
#include "../ActorComponent.h"

class Inventory : public ActorComponent
{
    int maxSlots;
    ItemSlot* slots;

public:

    Inventory(Actor* owner, int size = 20);
    ~Inventory();

    bool addItem(Item* item, int quantity = 1);
    bool removeItem(const std::string& name, Item*& removedItem, int quantity = 1);

    void showInventory() const;
};
