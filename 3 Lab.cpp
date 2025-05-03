#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character: " << character.name << ", HP: " << character.health
           << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }
};

class Weapon {
private:
    std::string name;
    int damage;

public:
    Weapon(const std::string& n, int d) : name(n), damage(d) {}

    int getDamage() const { return damage; }
    std::string getName() const { return name; }

    Weapon operator+(const Weapon& other) const {
        std::string newName = name + "-" + other.name;
        int newDamage = damage + other.damage;
        return Weapon(newName, newDamage);
    }

    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
        os << "Weapon: " << weapon.name << ", Damage: " << weapon.damage;
        return os;
    }
};

int main() {
    Character hero1("Hero", 100, 20, 10);
    Character hero2("Hero", 100, 20, 10);
    Character hero3("Warrior", 150, 25, 15);

    if (hero1 == hero2) {
        std::cout << "Hero1 and Hero2 are the same!\n";
    }
    if (!(hero1 == hero3)) {
        std::cout << "Hero1 and Hero3 are different!\n";
    }

    std::cout << hero1 << std::endl;

    Weapon sword("Sword", 50);
    Weapon bow("Bow", 30);

    Weapon combined = sword + bow;
    std::cout << "Combined weapon: " << combined << std::endl;

    if (sword > bow) {
        std::cout << sword.getName() << " is stronger than " << bow.getName() << std::endl;
    } else {
        std::cout << bow.getName() << " is stronger than " << sword.getName() << std::endl;
    }

    return 0;
}
