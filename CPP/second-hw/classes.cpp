#include <iostream>
#include <string>
using namespace std;

class character
{

	string name;
	float health;
	float max_health;
	bool is_dead;
	void UpdateIsDead() {
		this->is_dead = getHealth() <= 0;
		if (is_dead) {
			cout << this->name << " has died!" << endl;
		}
	}
	
public:
	character(string name, float health)
	{
		this->name = name;
		this->health = health;
		this->max_health = health;
		UpdateIsDead();
	}
	bool isDead() {
		return is_dead;
	}
	void getinfo()
	{
		cout << "Name: " << name << "; Health: " << health <<"/"<<max_health<<";\n";
	}
	float getHealth() {
		return health;
	}
	void setHealth(float health) {
		this->health = health;
		if (health < 0) {
			this->health = 0;
		}
		if (health > max_health) {
			this->health = max_health;
		}
		UpdateIsDead();
	}
	void heal(float heal) {
		setHealth(getHealth() + heal);
	}
	void damage(float damage) {
		setHealth(getHealth() - damage);
	}
	string getName() {
		return name;
	}

};

class warrior :public character
{
protected:
	float strength;
public:
	void attack(character* victim)
	{
		victim->damage(strength);
	}
	float getDamage() {
		return strength;
	}
	warrior(string name, float health, float strength): character(name, health){
		this->strength = strength;

	}
	void getinfo()
	{
		cout << "Name: " << getName() << "; Health: " << getHealth() << "; Strength: "<< getDamage() <<";\n";
	}
};

class zealot :public character
{
protected:
	float mana;
public:
	void heal(character* target, float heal)
	{
		if (heal > mana) {
			cout << "Not enough mana!" << endl;
			return;
		}
		if (target->isDead()) {
			cout << target->getName() << " is dead! Cannot heal." << endl;
			return;
		}
		target->heal(heal);
		this->mana -= heal;
	}
	float getMana() {
		return mana;
	}
	void getinfo()
	{
		cout << "Name: " << getName() << "; Health: " << getHealth() << "; Mana: " << getMana() << ";\n";
	}
	zealot(string name, float health, float mana) : character(name, health) {
		this->mana = mana;

	}
};
class necromancer : public zealot {
public:
	necromancer(string name, float health, float mana) : zealot(name, health, mana) {}
	void revive(character* target) {
		float reviveHealth = 20;
		if (getMana() < reviveHealth) {
			cout << "Not enough mana to revive!" << endl;
			return;
		}
		target->setHealth(reviveHealth);
		this->mana -= reviveHealth;
		cout << target->getName() << " has been revived with " << reviveHealth << " HP!" << endl;
	}
};
class thief : public warrior {
public:
	thief(string name, float health, float strength) : warrior(name, health, strength) {}
	void attack(character* victim)
	{
		victim->damage(strength/2);
		float lifesteal = (strength/2)*0.5;
		this->heal(lifesteal);
	}
};

int main()
{
	warrior w1("uno", 100, 90);
	warrior w2("dos", 100, 30);
	zealot zealot1("tres", 100, 10);
	necromancer necro1("cuatro", 100, 30);
	thief thief1("cinco", 80, 40);

	w1.getinfo();
	cout << endl;
	w2.getinfo();
	cout << "--------------------" << endl;

	w1.attack(&w2);
	w2.attack(&thief1);
	w1.getinfo();
	cout << endl;
	w2.getinfo();

	cout << "--------------------" << endl;
	zealot1.getinfo();
	zealot1.heal(&w1, 5);
	cout << endl;
	w1.getinfo();

	cout << "--------------------" << endl;
	thief1.getinfo();
	cout << "Thief attacks warrior w2" << endl;
	thief1.attack(&w2);
	cout << endl;
	thief1.getinfo();
	w2.getinfo();


	cout << "--------------------" << endl;
	necro1.getinfo();
	necro1.revive(&w2);
	cout << endl;
	w2.getinfo();
	necro1.getinfo();

	cout << "--------------------" << endl;
	zealot1.heal(&w2, 5);

	return 0;
}