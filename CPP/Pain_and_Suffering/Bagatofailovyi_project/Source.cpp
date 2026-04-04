#include <iostream>
#include <string>

#include "Public/Scene.h"
#include "Public/Actors/Character.h"
#include "Public/Actors/Chest.h"

using namespace std;

int main()
{
  Scene scene;
  
  Item sword("Steel Sword");
  Item axe("Vibranium Battle Axe");
  Item food("Mivina");

  Character FallenGod("Fallen God", &scene, { 1, 1 });
  Chest chest("Wooden Chest", &scene, { 1, 2 });

  FallenGod.getInventory()->addItem(&sword);
  FallenGod.getInventory()->addItem(&axe);
  FallenGod.getInventory()->addItem(&food, 5);

  FallenGod.showPosition();
  FallenGod.showInventory();
  cout << endl;

  chest.showInventory();
  cout << endl;

  FallenGod.putItemToChest("Wooden Chest", "Steel Sword");
  FallenGod.putItemToChest("Wooden Chest", "Mivina", 2);
  cout << endl;

  FallenGod.showInventory();
  cout << endl;

  chest.showInventory();
  cout << endl;

  FallenGod.takeItemFromChest("Wooden Chest", "Steel Sword");
  cout << endl;

  FallenGod.showInventory();
  cout << endl;

  chest.showInventory();

  return 0;
}
