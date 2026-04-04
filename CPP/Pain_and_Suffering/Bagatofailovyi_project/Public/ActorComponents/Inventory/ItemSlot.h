#pragma once

#include "../../Item.h"

class ItemSlot
{
    Item* item;
    int quantity;
    
public:
    ItemSlot() : item(nullptr), quantity(0) {}
    
    Item* getItem() const { return item; }
    int getQuantity() const { return quantity; }
    
    void setItem(Item* item) { this->item = item; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    
    void AddQuantity(int quantity) { this->quantity += quantity; }
    void DecreaseQuantity(int quantity) { this->quantity -= quantity; }
};
