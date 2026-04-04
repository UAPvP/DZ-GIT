#pragma once

#include <iostream>

#include "Actor.h"
#include "../ActorComponents/Inventory/Inventory.h"
#include "../ActorComponents/Cords.h"

using namespace std;

class Scene;
class Chest;

class Character : public Actor
{
    Inventory* inventory;
    Cords* cords;
    Scene* scene;

public:
    Character(std::string name, Scene* scene, const Vector2D& spawnPoint);

    void showInventory();
    void showPosition() const;
    void MoveTo(const Vector2D& newPosition);

    void dropItem(const std::string& name, int quantity = 1);
    void pickUpItem(const std::string& name);

    Chest* findChest(const string& chestName) const;
    
    bool canOpenChest(Chest* chest) const;

    void putItemToChest(const string& chestName, const string& itemName, int quantity = 1);
    void takeItemFromChest(const string& chestName, const string& itemName, int quantity = 1);

    Inventory* getInventory()
    {
        return inventory;
    }

    Cords* getCords() const
    {
        return cords;
    }

    ~Character();
};
