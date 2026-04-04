#include "../../../Public/ActorComponents/Inventory/Inventory.h"

#include <iostream>

Inventory::Inventory(Actor* owner, int size) : ActorComponent(owner)
{
    maxSlots = size;
    slots = new ItemSlot[maxSlots];
}

Inventory::~Inventory()
{
    delete[] slots;
}

bool Inventory::addItem(Item* item, int quantity)
{

    for (int i = 0; i < maxSlots; i++)
    {
        if (slots[i].getItem() != nullptr && slots[i].getItem()->getName() == item->getName())
        {
            slots[i].AddQuantity(quantity);
            return true;
        }
    }

    for (int i = 0; i < maxSlots; i++)
    {
        if (slots[i].getItem() == nullptr)
        {
            slots[i].setItem(item);
            slots[i].setQuantity(quantity);

            return true;
        }
    }

    std::cout << "Inventory is full!" << std::endl;
    return false;
}

bool Inventory::removeItem(const std::string& name, Item*& removedItem, int quantity)
{
    for (int i = 0; i < maxSlots; i++)
    {
        if (slots[i].getItem() != nullptr && slots[i].getItem()->getName() == name)
        {
            slots[i].DecreaseQuantity(quantity);
            removedItem = slots[i].getItem();

            if (slots[i].getQuantity() <= 0)
            {
                slots[i].setItem(nullptr);
                slots[i].setQuantity(0);
            }
            return true;
        }
    }
    return false;
}

void Inventory::showInventory() const
{
    for (int i = 0; i < maxSlots; i++)
    {
        if (slots[i].getItem() != nullptr)
        {
            std::cout << "* " << slots[i].getItem()->getName() << " x" << slots[i].getQuantity() << '\n';
        }
    }
}
