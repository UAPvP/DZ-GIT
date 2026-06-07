#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

using namespace std;

class player;

class hero
{
    int health_;
    int max_health_;
    int damage_;

public:
    int get_health() const { return health_; }
    int get_max_health() const { return max_health_; }
    int get_damage() const { return damage_; }

    bool IsAlive() const { return health_ > 0; }
    bool IsWounded() const { return IsAlive() && health_ < max_health_; }

    hero(const int health, const int damage) : health_(health), max_health_(health), damage_(damage) {}
    virtual ~hero() = default;

    virtual string get_name() const = 0;
    virtual void make_action(player* enemy, player* ally, vector<string>& history) = 0;

    void damage(int value)
    {
        health_ -= value;
        if (health_ < 0)
        {
            health_ = 0;
        }
    }

    void heal(int value)
    {
        health_ += value;
        if (health_ > max_health_)
        {
            health_ = max_health_;
        }
    }
};

class player
{
    vector<hero*> heroes_;

public:
    template<typename T, typename... Args>
    void add_hero(Args&&... args)
    {
        hero* new_hero = new T(std::forward<Args>(args)...);

        heroes_.push_back(new_hero);
    }

    const vector<hero*>& get_heroes() const { return heroes_; }

    void clear_heroes()
    {
        for (hero* hero : heroes_)
        {
            delete hero;
        }
    }

    virtual ~player()
    {
        clear_heroes();
    }

    bool has_alive_heroes() const
    {
        for (const hero* hero : heroes_)
        {
            if (hero->IsAlive())
            {
                return true;
            }
        }

        return false;
    }

    vector<hero*> get_alive_heroes() const
    {
        vector<hero*> alive_heroes;

        for (hero* hero : heroes_)
        {
            if (hero->IsAlive())
            {
                alive_heroes.push_back(hero);
            }
        }

        return alive_heroes;
    }

    vector<hero*> get_wounded_heroes() const
    {
        vector<hero*> wounded_heroes;

        for (hero* hero : heroes_)
        {
            if (hero->IsWounded())
            {
                wounded_heroes.push_back(hero);
            }
        }

        return wounded_heroes;
    }

    void take_turn(player* enemy, vector<string>& history)
    {
        for (size_t i = 0; i < heroes_.size(); i++)
        {
            hero* current_hero = heroes_.front();

            heroes_.erase(heroes_.begin());
            heroes_.push_back(current_hero);

            if (current_hero->IsAlive())
            {
                current_hero->make_action(enemy, this, history);
                return;
            }
        }

        history.push_back("Player has no alive heroes");
    }
};

class warrior : public hero
{
public:
    string get_name() const override { return "Warrior"; }
    void make_action(player* enemy, player* ally, vector<string>& history) override;

    warrior(const int health, const int damage) : hero(health, damage) {}
};

class mage : public hero
{
public:
    string get_name() const override { return "Mage"; }
    void make_action(player* enemy, player* ally, vector<string>& history) override;

    mage(const int health, const int damage) : hero(health, damage) {}
};

class robber : public hero
{
public:
    string get_name() const override { return "Robber"; }
    void make_action(player* enemy, player* ally, vector<string>& history) override;

    robber(const int health, const int damage) : hero(health, damage) {}
};

int random_index(const int max)
{
    return rand() % max;
}

bool random_chance(int percent)
{
    return (rand() % 100) < percent;
}

void warrior::make_action(player* enemy, player* ally, vector<string>& history)
{
    vector<hero*> targets = enemy->get_alive_heroes();

    if (targets.empty())
    {
        history.push_back(get_name() + " has nobody to attack");
        return;
    }

    hero* target = targets[random_index(static_cast<int>(targets.size()))];
    target->damage(get_damage());

    history.push_back(get_name() + " attacked " + target->get_name());
}

void mage::make_action(player* enemy, player* ally, vector<string>& history)
{
    vector<hero*> targets = ally->get_wounded_heroes();

    if (targets.empty())
    {
        history.push_back(get_name() + " found nobody to heal");
        return;
    }

    hero* target = targets[random_index(static_cast<int>(targets.size()))];
    target->heal(get_damage());

    history.push_back(get_name() + " healed " + target->get_name());
}

void robber::make_action(player* enemy, player* ally, vector<string>& history)
{
    vector<hero*> targets = enemy->get_alive_heroes();

    if (targets.empty())
    {
        history.push_back(get_name() + " has nobody to sneak up on");
        return;
    }

    hero* target = targets[random_index(static_cast<int>(targets.size()))];

    if (random_chance(50))
    {
        const int hidden_damage = get_damage() * 3;
        target->damage(hidden_damage);

        history.push_back(get_name() + " sneaked up on " + target->get_name() + " and dealt hidden damage");
        return;
    }

    damage(get_damage());
    history.push_back(get_name() + " was detected by " + target->get_name() + " and got damaged");
}

void print_player(const player& player, const string& name)
{
    cout << name << ":\n";

    for (const hero* hero : player.get_heroes())
    {
        cout << hero->get_name()
            << " hp: " << hero->get_health()
            << "/" << hero->get_max_health()
            << " damage: " << hero->get_damage()
            << "\n";
    }
}

void print_history(const vector<string>& history)
{
    cout << "History:\n";

    for (const string& event : history)
    {
        cout << event << "\n";
    }
}

int main(int argc, char* argv[])
{
    srand(time(nullptr));
    player player_1;
    player_1.add_hero<warrior>(10, 5);
    player_1.add_hero<mage>(10, 2);
    player_1.add_hero<robber>(10, 3);

    player player_2;
    player_2.add_hero<warrior>(13, 3);
    player_2.add_hero<mage>(12, 2);
    player_2.add_hero<robber>(15, 5);

    vector<string> history;

    while (player_1.has_alive_heroes() && player_2.has_alive_heroes())
    {
        system("cls");

        player_1.take_turn(&player_2, history);

        if (!player_2.has_alive_heroes())
        {
            history.push_back("All heroes of Player 2 died. Player 2 lost");
        }
        else
        {
            player_2.take_turn(&player_1, history);

            if (!player_1.has_alive_heroes())
            {
                history.push_back("All heroes of Player 1 died. Player 1 lost");
            }
        }

        print_player(player_1, "Player 1");
        cout << "\n";
        print_player(player_2, "Player 2");
        cout << "\n";
        print_history(history);

        if (!player_1.has_alive_heroes() || !player_2.has_alive_heroes())
        {
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
