#include "../../Public/Actors/Chest.h"
#include "../../Public/Scene.h"
using namespace std;

Chest::Chest(string name, Scene* scene, const Vector2D& spawnPoint) : Actor("Chest", name)
{
	this->scene = scene;
	inventory = new Inventory(this);
	cords = new Cords(this, spawnPoint);

	scene->AddActor(this);
}

void Chest::showInventory() const
{
	cout << name << "'s inventory: " << endl;
	inventory->showInventory();
}

void Chest::storeItem(Item* item, int quantity)
{
	inventory->addItem(item, quantity);
}

bool Chest::takeItem(const string& name, Item*& item, int quantity)
{
	return inventory->removeItem(name, item, quantity);
}

Chest::~Chest()
{
	delete inventory;
	delete cords;
}
