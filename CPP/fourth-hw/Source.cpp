#include <iostream>
#include <string>
using namespace std;

class Item {
protected:
    string name;
public:
    Item(string name) : name(name) {}
    virtual ~Item() {}

    string getName() const {
        return name;
    }
};
class Wood : public Item {
public:
    Wood() : Item("Wood") {}
};
class Stone : public Item {
public:
    Stone() : Item("Stone") {}
};
class Diamond : public Item {
public:
    Diamond() : Item("Diamond") {}
};

class ItemSlot {
public:
    Item* item;
    int count;

    ItemSlot() {
        item = nullptr;
        count = 0;
    }
};

class Inventory {
    int slots_num;
    ItemSlot* slots;
public:
    Inventory() {
        slots_num = 0;
        slots = nullptr;
    }
    Inventory(int num) {
        init(num);
    }
    void init(int num) {
        slots_num = num;
        slots = new ItemSlot[slots_num];
    }
    void addItem(Item* item) {
        //необ. стаки
        for (int i = 0; i < slots_num; i++) {
            if (slots[i].item != nullptr &&
                slots[i].item->getName() == item->getName() &&
                slots[i].count < 10) {
                slots[i].count++;
                return;
            }
        }
        //пошук слота
        for (int i = 0; i < slots_num; i++) {
            if (slots[i].item == nullptr) {
                slots[i].item = item;
                slots[i].count = 1;
                return;
            }
        }
        cout << "Inventory full!\n";
    }
    void removeItem(int index) {
        if (index < 0 || index >= slots_num) {
            cout << "Invalid slot\n";
            return;
        }
        if (slots[index].item == nullptr) {
            cout << "Slot empty\n";
            return;
        }
        slots[index].item = nullptr;
        slots[index].count = 0;
    }
    void show() {
        for (int i = 0; i < slots_num; i++) {
            if (slots[i].item)
                cout << i << ": " << slots[i].item->getName()
                << " x" << slots[i].count << endl;
            else
                cout << i << ": [empty]\n";
        }
    }
    ~Inventory() {
        delete[] slots;
    }
};

class Actor {
public:
    virtual void info() = 0;
    virtual ~Actor() {}
};

class Character : public Actor {
    string name;
    Inventory inventory;

public:
    Character(string name, int slots) {
        this->name = name;
        inventory.init(slots);
    }
    void addItem(Item* item) {
        inventory.addItem(item);
    }
    void showInventory() {
        cout << "Character: " << name << endl;
        inventory.show();
    }
    void info() override {
        cout << "Character: " << name << endl;
    }
    Inventory& getInventory() {
        return inventory;
    }
};

class DroppedItem : public Actor {
    Item* item;
public:
    DroppedItem(Item* item) {
        this->item = item;
    }
    Item* getItem() {
        return item;
    }
    void info() override {
        cout << "Dropped item: " << item->getName() << endl;
    }
};
class Actors {
    Actor** data;
    int capacity;
    int count;
public:
    Actors(int capacity = 10) {
        this->capacity = capacity;
        count = 0;
        data = new Actor * [capacity];
    }
    void AddActor(Actor* actor) {
        if (count >= capacity) {
            cout << "Actors full!\n";
            return;
        }
        data[count++] = actor;
    }
    void ShowAll() {
        for (int i = 0; i < count; i++) {
            data[i]->info();
        }
    }
    ~Actors() {
        for (int i = 0; i < count; i++) {
            delete data[i];
        }
        delete[] data;
    }
};
class Scene {
    Actors* actors;
public:
    Scene() {
        actors = new Actors(10);
    }
    void AddActor(Actor* actor) {
        actors->AddActor(actor);
    }
    void Show() {
        cout << "\nScene:\n";
        actors->ShowAll();
    }
    ~Scene() {
        delete actors;
    }
};

int main() {
    Scene scene;
    Character* player = new Character("Slava", 3);
    scene.AddActor(player);

    Wood* wood = new Wood();
    Stone* stone = new Stone();
    Diamond* diamond = new Diamond();

    player->addItem(wood);
    player->addItem(stone);
    player->addItem(wood);
    player->showInventory();
    cout << "-------------------------="<<endl;
    DroppedItem* drop = new DroppedItem(diamond);
    scene.AddActor(drop);
    scene.Show();
    return 0;
}