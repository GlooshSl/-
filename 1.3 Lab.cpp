#include <iostream>
#include <string>
#include <ctime>

class Entity {
protected:
    std::string name;
    int health;
    int attackPower;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attackPower(a), defense(d) {}

    virtual ~Entity() {}

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getDefense() const { return defense; }

    void reduceHealth(int amount) { health -= amount; }

    virtual void attack(Entity& target) = 0;

    virtual void heal(int amount) {
        std::cout << name << " cannot heal!\n";
    }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 20) { // Критический удар (20%)
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void heal(int amount) override {
        health += amount;
        if (health > 100) {
            health == 100;
        }
        std::cout << name << " heals for " << amount << " HP!\n";
    }

    void displayInfo() const override {
        std::cout << "Character: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 30) { // Ядовитая атака (30%)
                damage += 5;
                std::cout << "Poisonous attack! ";
            }
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

// Новый класс Boss, унаследованный от Monster
class Boss : public Monster {
public:
    Boss(const std::string& n, int h, int a, int d)
        : Monster(n, h, a, d) {}

    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 40) { // Огненный удар (40% вероятность)
                damage += 10;
                std::cout << "Fire Strike! ";
            }
            target.reduceHealth(damage);
            std::cout << name << " unleashes a powerful attack on " << target.getName()
                << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Boss: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss dragon("Dragon", 200, 30, 15);

    Entity* entities[] = { &hero, &goblin, &dragon };

    // Вывод информации о всех сущностях
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    // Бой
    hero.attack(goblin);
    goblin.attack(hero);
    dragon.attack(hero);

    // Лечение персонажа
    std::cout << "\nHero decides to heal...\n";
    hero.heal(30);

    // Вывод обновлённой информации
    std::cout << "\nUpdated Stats:\n";
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    return 0;
}
