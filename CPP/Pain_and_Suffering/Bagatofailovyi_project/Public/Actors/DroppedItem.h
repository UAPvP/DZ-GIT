#pragma once

#include "Actor.h"
#include "../ActorComponents/Cords.h"

class Item;

class DroppedItem : public Actor
{
    Item* item;
    int quantity;
    Cords* cords;

public:

    DroppedItem(Item* item, int quantity, const Vector2D& spawnPoint) : Actor("DroppedItem", "DroppedItem")
    {
        this->item = item;
        this->quantity = quantity;
        cords = new Cords(this, spawnPoint);
    }

    Item* getItem() const
    {
        return item;
    }

    int getQuantity() const
    {
        return quantity;
    }

    Cords* getCords() const
    {
        return cords;
    }

    ~DroppedItem()
    {
        delete cords;
    }
};
