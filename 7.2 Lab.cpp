#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>

class Fighter {
private:
    std::string fighterName;
    int hitPoints;
    int attackPower;
    std::mutex combatLock; 

public:
    Fighter(const std::string& name, int hp, int attack)
        : fighterName(name), hitPoints(hp), attackPower(attack) {}

    void receiveDamage(int damage) {
        std::lock_guard<std::mutex> lock(combatLock);
        hitPoints -= damage;
        if (hitPoints < 0) hitPoints = 0;
    }

    int getAttackPower() const {
        return attackPower;
    }

    std::string getName() const {
        return fighterName;
    }

    bool isStillAlive() const {
        return hitPoints > 0;
    }

    void showStatus() {
        std::lock_guard<std::mutex> lock(combatLock);
        std::cout << fighterName << " (HP: " << hitPoints << ")" << std::endl;
    }
};

void simulateCombat(Fighter& firstFighter, Fighter& secondFighter) {
    std::random_device randomDevice;
    std::mt19937 randomEngine(randomDevice());
    std::uniform_int_distribution<> damageRange(1, firstFighter.getAttackPower());

    while (firstFighter.isStillAlive() && secondFighter.isStillAlive()) {
        int damageDealt = damageRange(randomEngine);

        secondFighter.receiveDamage(damageDealt);

        std::cout << firstFighter.getName() << " attacks " << secondFighter.getName()
                  << " dealing " << damageDealt << " damage!" << std::endl;

        firstFighter.showStatus();
        secondFighter.showStatus();
        std::cout << "----------------------" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::swap(firstFighter, secondFighter);
    }

    if (firstFighter.isStillAlive()) {
        std::cout << firstFighter.getName() << " is victorious!" << std::endl;
    } else {
        std::cout << secondFighter.getName() << " is victorious!" << std::endl;
    }
}

int main() {
    Fighter player("Knight", 100, 20);
    Fighter enemy("Dragon", 150, 15);

    std::cout << "Battle begins: " << player.getName() 
              << " vs " << enemy.getName() << "!" << std::endl;

    std::thread combatThread(simulateCombat, std::ref(player), std::ref(enemy));
    combatThread.join();

    return 0;
}
