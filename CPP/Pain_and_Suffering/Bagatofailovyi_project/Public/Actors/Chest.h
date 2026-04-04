#pragma once

#include <iostream>
#include <string>

#include "Actor.h"
#include "../ActorComponents/Cords.h"
#include "../ActorComponents/Inventory/Inventory.h"

using namespace std;

class Scene;

class Chest : public Actor
{
	Inventory* inventory;
	Cords* cords;
	Scene* scene;

public:

	Chest(string name, Scene* scene, const Vector2D& spawnPoint);

	void showInventory() const;

	void storeItem(Item* item, int quantity = 1);
	bool takeItem(const string& name, Item*& item, int quantity = 1);

	Inventory* getInventory() const
	{
		return inventory;
	}

	Cords* getCords() const
	{
		return cords;
	}

	~Chest();
};