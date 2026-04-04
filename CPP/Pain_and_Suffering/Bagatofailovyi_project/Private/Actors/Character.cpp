#include "../../Public/Actors/Character.h"

#include "../../Public/Scene.h"
#include "../../Public/Actors/DroppedItem.h"
#include "../../Public/Actors/Chest.h"

using namespace std;

Character::Character(string name, Scene* scene, const Vector2D& spawnPoint) : Actor("Character", name)
{
    this->scene = scene;
    inventory = new Inventory(this);
    cords = new Cords(this, spawnPoint);

    scene->AddActor(this);
}

void Character::showInventory()
{
    cout << name << "'s inventory: " << endl;
    inventory->showInventory();
}

void Character::showPosition() const
{
    Vector2D pos = cords->getPosition();
    cout << name << " position: " << pos.x << ", " << pos.y << endl;
}

void Character::MoveTo(const Vector2D& newPosition)
{
    cords->setPosition(newPosition);
}

void Character::dropItem(const string& name, int quantity)
{
    Item* item;

    if (inventory->removeItem(name, item, quantity))
    {
        scene->AddActor(new DroppedItem(item, quantity, cords->getPosition()));
        cout << "Dropped " << name << " x" << quantity << endl;
    }
    else
    {
        cout << "No such item to drop!" << endl;
    }
}

void Character::pickUpItem(const string& name)
{
    Actor** actors = scene->getActors();
    int count = scene->getActorsCount();

    for (int i = 0; i < count; i++)
    {
        if (actors[i]->getType() == "DroppedItem")
        {
            DroppedItem* dropped = (DroppedItem*)actors[i];

            if (dropped->getItem()->getName() == name && cords->hasSamePosition(*dropped->getCords()))
            {
                int droppedQuantity = dropped->getQuantity();
                inventory->addItem(dropped->getItem(), droppedQuantity);

                scene->RemoveActor(i);
                delete dropped;

                cout << "Picked up " << name << " x" << droppedQuantity << endl;
                return;
            }
        }
    }
    
    cout << "No such item on the ground!" << endl;
}

Chest* Character::findChest(const string& chestName) const
{
    Actor** actors = scene->getActors();
    int count = scene->getActorsCount();

    for (int i = 0; i < count; i++)
    {
        if (actors[i]->getType() == "Chest" && actors[i]->getName() == chestName)
        {
            return (Chest*)actors[i];
        }
    }

    return nullptr;
}

bool Character::canOpenChest(Chest* chest) const
{
    if (chest == nullptr)
    {
        return false;
    }

    return cords->isNear(*chest->getCords());
}

void Character::putItemToChest(const string& chestName, const string& itemName, int quantity)
{
    Chest* chest = findChest(chestName);

    if (chest == nullptr)
    {
        cout << "Chest not found!" << endl;
        return;
    }

    if (!canOpenChest(chest))
    {
        cout << "Too far from chest!" << endl;
        return;
    }

    Item* item;

    if (inventory->removeItem(itemName, item, quantity))
    {
        chest->storeItem(item, quantity);
        cout << itemName << " x" << quantity << " put into " << chestName << endl;
    }
    else
    {
        cout << "No such item in inventory!" << endl;
    }
}

void Character::takeItemFromChest(const string& chestName, const string& itemName, int quantity)
{
    Chest* chest = findChest(chestName);

    if (chest == nullptr)
    {
        cout << "Chest not found!" << endl;
        return;
    }

    if (!canOpenChest(chest))
    {
        cout << "Too far from chest!" << endl;
        return;
    }

    Item* item;

    if (chest->takeItem(itemName, item, quantity))
    {
        inventory->addItem(item, quantity);
        cout << "Took " << itemName << " x" << quantity << " from " << chestName << endl;
    }
    else
    {
        cout << "No such item in chest!" << endl;
    }
}

Character::~Character()
{
    delete inventory;
    delete cords;
}
