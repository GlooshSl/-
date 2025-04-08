#include <iostream>
#include <string>
using namespace std;

class Character {
private:
    string name;  // Приватное поле: имя персонажа
    int health;        // Приватное поле: уровень здоровья
    int attack;        // Приватное поле: уровень атаки
    int defense;       // Приватное поле: уровень защиты
    const int maxHealth = 100;

public:
    // Конструктор для инициализации данных
    Character(const string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Метод для получения уровня здоровья
    int getHealth() const {
        return health;
    }

    // Метод для вывода информации о персонаже
    void displayInfo() const {
        cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << endl;
    }

    // Метод для атаки другого персонажа
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << endl;
        }
        else {
            cout << name << " attacks " << enemy.name << ", but it has no effect!" << endl;
        }
    }
    void heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth;
        }
        cout << name << " heals for " << amount << " HP! Current HP: " << health << endl;
    }

    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0;
        }
        cout << name << " takes " << amount << " damage! Current HP: " << health << endl;
    }
};

int main() {
    // Создаем объекты персонажей
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // Выводим информацию о персонажах
    hero.displayInfo();
    monster.displayInfo();

    // Герой атакует монстра
    hero.attackEnemy(monster);
    monster.displayInfo();

    hero.takeDamage(30);
    hero.heal(20);

    return 0;
}
