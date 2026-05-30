#include <chrono>
#include <iostream>
#include <map>
#include <thread>
#include <vector>

using namespace std;

class Character;
class Item;
struct EffectContext
{
    int duration;

    EffectContext(int inDuration)
    {
        duration = inDuration;
    }

    virtual ~EffectContext() = default;
};

class Effect
{
    Character* owner;
    int duration;
    int lasted;

    bool ended = false;

protected:
    virtual void OnTick() {}
    virtual void OnEnd() {}

public:
    Character* GetOwner() const { return owner; }

    bool IsEnded() const { return ended; }

    Effect(Character* inOwner, const EffectContext& inContext)
    {
        owner = inOwner;
        duration = inContext.duration;
        lasted = 0;
    }

    void End()
    {
        if (ended) return;

        ended = true;

        OnEnd();
    }

    virtual ~Effect()
    {
        End();
    }

    void Tick()
    {
        if (IsEnded()) return;

        if (duration <= lasted)
        {
            End();
            return;
        }

        OnTick();
        ++lasted;
    }
};

struct CharacterContext
{
    int health = 0;
    int stamina = 0;
    int mana = 0;
    int damage = 0;
    int hunger = 0;
};

class Character
{
    map<string, float> properties;
    vector<Item*> items;
    vector<Effect*> effects;

public:
    float& Property(const string& name) { return properties[name]; }

    void TakeDamage(int dmg)
    {
        Property("Vitality.Health.Value") -= dmg;
        if (Property("Vitality.Health.Value") < 0)
            Property("Vitality.Health.Value") = 0;
    }
    Character(const CharacterContext& inContext)
    {
        Property("Vitality.Health.Value") = Property("Vitality.Health.Max") = inContext.health;
        Property("Vitality.Stamina.Value") = Property("Vitality.Stamina.Max") = inContext.stamina;
        Property("Vitality.Mana.Value") = Property("Vitality.Mana.Max") = inContext.mana;
        Property("Vitality.Damage.Value") = inContext.damage;
        Property("Vitality.Hunger.Value") = Property("Vitality.Hunger.Max")=inContext.hunger;
    }

    void Tick()
    {
        for (Effect* effect : effects)
        {
            effect->Tick();

            if (effect->IsEnded())
            {
                delete effect;

                std::erase(effects, effect);
            }
        }
    }

    template <typename T>
        requires std::derived_from<T, Effect>
    void AddEffect(const EffectContext& inContext)
    {
        effects.push_back(new T(this, inContext));
    }

    void Print() const
    {
        for (const auto& property : properties)
        {
            cout << " * " << property.first << " - " << property.second << '\n';
        }
    }
};

struct StrengthEffectContext : EffectContext
{
    int bonus = 0;

    StrengthEffectContext(int inDuration, int inBonus) : EffectContext(inDuration)
    {
        bonus = inBonus;
    }
};

class StrengthEffect : public Effect
{
    int bonus = 0;

public:
    StrengthEffect(Character* inOwner, const EffectContext& inContext) : Effect(inOwner, inContext)
    {
        const StrengthEffectContext& castedContext = static_cast<const StrengthEffectContext&>(inContext);

        bonus = castedContext.bonus;

        GetOwner()->Property("Vitality.Damage.Bonus") += bonus;
    }

    void OnEnd() override
    {
        GetOwner()->Property("Vitality.Damage.Bonus") -= bonus;
    }

    ~StrengthEffect() override = default;
};
struct RegenEffectContext : EffectContext
{
    int regen = 0;

    RegenEffectContext(int inDuration, int inBonus) : EffectContext(inDuration)
    {
        regen = inBonus;
    }
};
class RegenEffect : public Effect
{
    int regen = 0;

public:
    RegenEffect(Character* inOwner, const EffectContext& inContext) : Effect(inOwner, inContext)
    {
        const RegenEffectContext& castedContext = static_cast<const RegenEffectContext&>(inContext);

        regen = castedContext.regen;
        if (inContext.duration < 1) {
            GetOwner()->Property("Vitality.Health.Value") += regen;
            if (GetOwner()->Property("Vitality.Health.Value") > GetOwner()->Property("Vitality.Health.Max")) GetOwner()->Property("Vitality.Health.Value") = GetOwner()->Property("Vitality.Health.Max");
            End();
        }
    }
    void OnTick() {
        if (GetOwner()->Property("Vitality.Health.Value") == GetOwner()->Property("Vitality.Health.Max")) return;
        GetOwner()->Property("Vitality.Health.Value") += regen;
        if (GetOwner()->Property("Vitality.Health.Value") > GetOwner()->Property("Vitality.Health.Max")) GetOwner()->Property("Vitality.Health.Value") = GetOwner()->Property("Vitality.Health.Max");
    }


    ~RegenEffect() override = default;
};
struct PoisonEffectContext : EffectContext
{
    int damagePerTick = 0;

    PoisonEffectContext(int duration, int dmg)
        : EffectContext(duration), damagePerTick(dmg) {
    }
};
class PoisonEffect : public Effect
{
    int dmg = 0;

public:
    PoisonEffect(Character* owner, const EffectContext& ctx)
        : Effect(owner, ctx)
    {
        const PoisonEffectContext& c =
            static_cast<const PoisonEffectContext&>(ctx);

        dmg = c.damagePerTick;
    }

protected:
    void OnTick() override
    {
        GetOwner()->TakeDamage(dmg);
    }
};
class Item {
protected:
    Character* owner = nullptr;
public:
    Item(Character* inOwner) : owner(inOwner) {}
    virtual void onUse() {};
    virtual ~Item() = default;
};
class Potion : public Item {
public:
    Potion(Character* owner) : Item(owner) {}
};
class StrengthPotion : public Potion {
public:
    StrengthPotion(Character* owner) : Potion(owner) {}

    void onUse() override {
        owner->AddEffect<StrengthEffect>(StrengthEffectContext(10, 5));
    }
};
class RegenPotion : public Potion {
public:
    RegenPotion(Character* owner) : Potion(owner) {}

    void onUse() override {
        owner->AddEffect<RegenEffect>(RegenEffectContext(10, 5));
    }
};

class Food : public Item {
    int replenish;
    void onUse() override {
        owner->Property("Vitality.Hunger.Value") += replenish;
    }
};
class Weapon : public Item
{
protected:
    int damage = 0;

public:
    Weapon(Character* owner, int dmg) : Item(owner), damage(dmg) {
    }

    virtual void Attack(Character* target) = 0;
};
class MagicSword : public Weapon
{
public:
    MagicSword(Character* owner, int dmg) : Weapon(owner, dmg) {
    }

    void Attack(Character* target) override
    {
        target->TakeDamage(damage);

        target->AddEffect<PoisonEffect>(PoisonEffectContext(5, 2));
    }
};

int main(int argc, char* argv[])
{
    CharacterContext ctx;
    ctx.health = 100;
    ctx.stamina = 100;
    ctx.mana = 100;
    ctx.damage = 10;
    ctx.hunger = 100;

    Character* hero = new Character(ctx);
    Character* enemy = new Character(ctx);

    MagicSword sword(hero, 15);

    for (int i = 0; i < 20; i++)
    {
        if (i == 3)
        {
            cout << "\n>>> Hero attacks Enemy with Magic Sword!\n";
            sword.Attack(enemy);
        }

        if (i == 1)
        {
            hero->AddEffect<StrengthEffect>(StrengthEffectContext(5, 10));
        }
        hero->Tick();
        enemy->Tick();

        system("cls");

        cout << "========== WORLD TICK: " << i << " ==========\n\n";

        cout << "HERO:\n";
        hero->Print();

        cout << "\nENEMY:\n";
        enemy->Print();

        cout << "\nENEMY HP: "
            << enemy->Property("Vitality.Health.Value") << "\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    delete hero;
    delete enemy;

    return 0;
}
