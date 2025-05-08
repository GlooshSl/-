#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <ctime>

template<typename T>
class EventLogger {
private:
    std::ofstream logStream;
    
public:
    EventLogger(const std::string& logFilename) {
        logStream.open(logFilename, std::ios::app);
        if (!logStream) {
            throw std::runtime_error("Не удалось открыть файл журнала");
        }
    }
    
    ~EventLogger() {
        if (logStream.is_open()) {
            logStream.close();
        }
    }
    
    void recordEvent(const T& eventMessage) {
        time_t currentTime = time(0);
        char* timeString = ctime(&currentTime);
        logStream << timeString << ": " << eventMessage << std::endl;
    }
};

class GameWorldException : public std::runtime_error {
public:
    GameWorldException(const std::string& message) : std::runtime_error(message) {}
};

class GameItem {
protected:
    std::string itemName;
    std::string itemDescription;
    
public:
    GameItem(const std::string& name, const std::string& description) 
        : itemName(name), itemDescription(description) {}
        
    virtual ~GameItem() {}
    
    virtual void activate() = 0;
    virtual std::string saveData() const = 0;
    virtual void loadData(const std::string& savedData) = 0;
    
    std::string getName() const { return itemName; }
    std::string getDescription() const { return itemDescription; }
};

class CombatGear : public GameItem {
private:
    int damageBonus;
    
public:
    CombatGear(const std::string& name, const std::string& desc, int bonus)
        : GameItem(name, desc), damageBonus(bonus) {}
        
    void activate() override {
        std::cout << "Экипировано: " << itemName << " (+" << damageBonus << " к урону)" << std::endl;
    }
    
    int getDamageBonus() const { return damageBonus; }
    
    std::string saveData() const override {
        return "Оружие," + itemName + "," + itemDescription + "," + std::to_string(damageBonus);
    }
    
    void loadData(const std::string& savedData) override {
        size_t firstDelim = savedData.find(',');
        size_t secondDelim = savedData.find(',', firstDelim+1);
        size_t thirdDelim = savedData.find(',', secondDelim+1);
        
        if (firstDelim == std::string::npos || secondDelim == std::string::npos || thirdDelim == std::string::npos) {
            throw GameWorldException("Некорректные данные снаряжения");
        }
        
        itemName = savedData.substr(firstDelim+1, secondDelim-firstDelim-1);
        itemDescription = savedData.substr(secondDelim+1, thirdDelim-secondDelim-1);
        damageBonus = std::stoi(savedData.substr(thirdDelim+1));
    }
};


class HealingItem : public GameItem {
private:
    int healthRestore;
    
public:
    HealingItem(const std::string& name, const std::string& desc, int heal)
        : GameItem(name, desc), healthRestore(heal) {}
        
    void activate() override {
        std::cout << "Использовано: " << itemName << " (восстанавливает " << healthRestore << " здоровья)" << std::endl;
    }
    
    int getHealAmount() const { return healthRestore; }
    
    std::string saveData() const override {
        return "Зелье," + itemName + "," + itemDescription + "," + std::to_string(healthRestore);
    }
    
    void loadData(const std::string& savedData) override {
        size_t firstDelim = savedData.find(',');
        size_t secondDelim = savedData.find(',', firstDelim+1);
        size_t thirdDelim = savedData.find(',', secondDelim+1);
        
        if (firstDelim == std::string::npos || secondDelim == std::string::npos || thirdDelim == std::string::npos) {
            throw GameWorldException("Некорректные данные зелья");
        }
        
        itemName = savedData.substr(firstDelim+1, secondDelim-firstDelim-1);
        itemDescription = savedData.substr(secondDelim+1, thirdDelim-secondDelim-1);
        healthRestore = std::stoi(savedData.substr(thirdDelim+1));
    }
};

class ItemStorage {
private:
    std::vector<std::unique_ptr<GameItem>> storedItems;
    
public:
    void addItem(std::unique_ptr<GameItem> newItem) {
        storedItems.push_back(std::move(newItem));
    }
    
    void removeItem(const std::string& itemToRemove) {
        for (auto it = storedItems.begin(); it != storedItems.end(); ++it) {
            if ((*it)->getName() == itemToRemove) {
                storedItems.erase(it);
                return;
            }
        }
        throw GameWorldException("Предмет не найден в инвентаре");
    }
    
    GameItem* findItem(const std::string& itemName) {
        for (auto& item : storedItems) {
            if (item->getName() == itemName) {
                return item.get();
            }
        }
        return nullptr;
    }
    
    void displayContents() const {
        if (storedItems.empty()) {
            std::cout << "Инвентарь пуст" << std::endl;
            return;
        }
        
        std::cout << "Содержимое инвентаря:" << std::endl;
        for (const auto& item : storedItems) {
            std::cout << "• " << item->getName() << ": " << item->getDescription() << std::endl;
        }
    }
    
    void saveInventory(const std::string& storageFile) const {
        std::ofstream outputFile(storageFile);
        if (!outputFile) {
            throw GameWorldException("Ошибка сохранения инвентаря");
        }
        
        for (const auto& item : storedItems) {
            outputFile << item->saveData() << "\n";
        }
    }
    
    void loadInventory(const std::string& storageFile) {
        std::ifstream inputFile(storageFile);
        if (!inputFile) {
            throw GameWorldException("Ошибка загрузки инвентаря");
        }
        
        storedItems.clear();
        std::string dataLine;
        while (std::getline(inputFile, dataLine)) {
            size_t typePos = dataLine.find(',');
            if (typePos == std::string::npos) continue;
            
            std::string itemType = dataLine.substr(0, typePos);
            std::unique_ptr<GameItem> loadedItem;
            
            if (itemType == "Оружие") {
                loadedItem = std::make_unique<CombatGear>("", "", 0);
            } else if (itemType == "Зелье") {
                loadedItem = std::make_unique<HealingItem>("", "", 0);
            } else {
                continue;
            }
            
            loadedItem->loadData(dataLine);
            storedItems.push_back(std::move(loadedItem));
        }
    }
};

class GameCreature {
protected:
    std::string creatureName;
    int hitPoints;
    int attackPower;
    int defenseValue;
    
public:
    GameCreature(const std::string& name, int hp, int atk, int def)
        : creatureName(name), hitPoints(hp), attackPower(atk), defenseValue(def) {}
        
    virtual ~GameCreature() {}
    
    virtual void performAttack(class GameCharacter& target) = 0;
    
    bool isAlive() const { return hitPoints > 0; }
    
    void receiveDamage(int damage) {
        hitPoints -= damage;
        if (hitPoints < 0) hitPoints = 0;
    }
    
    virtual std::string saveCreatureData() const {
        return creatureName + "," + std::to_string(hitPoints) + "," + 
               std::to_string(attackPower) + "," + std::to_string(defenseValue);
    }
    
    virtual void loadCreatureData(const std::string& creatureData) {
        size_t firstDelim = creatureData.find(',');
        size_t secondDelim = creatureData.find(',', firstDelim+1);
        size_t thirdDelim = creatureData.find(',', secondDelim+1);
        
        if (firstDelim == std::string::npos || secondDelim == std::string::npos || thirdDelim == std::string::npos) {
            throw GameWorldException("Некорректные данные существа");
        }
        
        creatureName = creatureData.substr(0, firstDelim);
        hitPoints = std::stoi(creatureData.substr(firstDelim+1, secondDelim-firstDelim-1));
        attackPower = std::stoi(creatureData.substr(secondDelim+1, thirdDelim-secondDelim-1));
        defenseValue = std::stoi(creatureData.substr(thirdDelim+1));
    }
    
    virtual void displayCreatureInfo() const {
        std::cout << creatureName << " - Здоровье: " << hitPoints 
                  << ", Атака: " << attackPower << ", Защита: " << defenseValue;
    }
    
    std::string getName() const { return creatureName; }
    int getHealth() const { return hitPoints; }
    int getAttack() const { return attackPower; }
    int getDefense() const { return defenseValue; }
};

class ForestGoblin : public GameCreature {
public:
    ForestGoblin() : GameCreature("Лесной гоблин", 30, 8, 3) {}
    
    void performAttack(GameCharacter& target) override {
        std::cout << "Гоблин атакует своим дубинкой!" << std::endl;
        int damageDealt = attackPower - target.getDefense();
        if (damageDealt > 0) {
            target.receiveDamage(damageDealt);
            std::cout << "Нанесено " << damageDealt << " урона!" << std::endl;
        } else {
            std::cout << "Атака не пробила защиту!" << std::endl;
        }
    }
    
    std::string saveCreatureData() const override {
        return "Гоблин," + GameCreature::saveCreatureData();
    }
};

class AncientDragon : public GameCreature {
public:
    AncientDragon() : GameCreature("Древний дракон", 100, 20, 10) {}
    
    void performAttack(GameCharacter& target) override {
        std::cout << "Дракон извергает пламя!" << std::endl;
        int damageDealt = attackPower - target.getDefense();
        if (damageDealt > 0) {
            target.receiveDamage(damageDealt);
            std::cout << "Нанесено " << damageDealt << " урона!" << std::endl;
        } else {
            std::cout << "Атака не пробила защиту!" << std::endl;
        }
    }
    
    std::string saveCreatureData() const override {
        return "Дракон," + GameCreature::saveCreatureData();
    }
};

class UndeadWarrior : public GameCreature {
public:
    UndeadWarrior() : GameCreature("Нежить-воин", 40, 10, 5) {}
    
    void performAttack(GameCharacter& target) override {
        std::cout << "Нежить атакует ржавым мечом!" << std::endl;
        int damageDealt = attackPower - target.getDefense();
        if (damageDealt > 0) {
            target.receiveDamage(damageDealt);
            std::cout << "Нанесено " << damageDealt << " урона!" << std::endl;
        } else {
            std::cout << "Атака не пробила защиту!" << std::endl;
        }
    }
    
    std::string saveCreatureData() const override {
        return "Нежить," + GameCreature::saveCreatureData();
    }
};

class GameCharacter {
private:
    std::string characterName;
    int currentHealth;
    int maximumHealth;
    int attackStat;
    int defenseStat;
    int characterLevel;
    int experiencePoints;
    ItemStorage characterInventory;
    
public:
    GameCharacter(const std::string& name, int health, int attack, int defense)
        : characterName(name), currentHealth(health), maximumHealth(health), 
          attackStat(attack), defenseStat(defense), characterLevel(1), experiencePoints(0) {}
        
    void attackTarget(GameCreature& enemy) {
        int damageDealt = attackStat - enemy.getDefense();
        if (damageDealt > 0) {
            enemy.receiveDamage(damageDealt);
            std::cout << characterName << " атакует " << enemy.getName() << " и наносит " << damageDealt << " урона!" << std::endl;
        } else {
            std::cout << characterName << " атакует " << enemy.getName() << ", но атака неэффективна!" << std::endl;
        }
    }
    
    void receiveDamage(int damageTaken) {
        currentHealth -= damageTaken;
        if (currentHealth < 0) {
            currentHealth = 0;
            throw GameWorldException(characterName + " повержен!");
        }
    }
    
    void restoreHealth(int healAmount) {
        currentHealth += healAmount;
        if (currentHealth > maximumHealth) currentHealth = maximumHealth;
        std::cout << characterName << " восстанавливает " << healAmount << " здоровья!" << std::endl;
    }
    
    void gainExperience(int expGained) {
        experiencePoints += expGained;
        if (experiencePoints >= 100) {
            characterLevel++;
            experiencePoints -= 100;
            maximumHealth += 10;
            currentHealth = maximumHealth;
            attackStat += 2;
            defenseStat += 1;
            std::cout << characterName << " достиг уровня " << characterLevel << "!" << std::endl;
        }
    }
    
    void useInventoryItem(const std::string& itemName) {
        GameItem* item = characterInventory.findItem(itemName);
        if (!item) {
            throw GameWorldException("Предмет не найден в инвентаре");
        }
        
        if (auto healingItem = dynamic_cast<HealingItem*>(item)) {
            restoreHealth(healingItem->getHealAmount());
            characterInventory.removeItem(itemName);
        } else if (auto weaponItem = dynamic_cast<CombatGear*>(item)) {
            attackStat += weaponItem->getDamageBonus();
            std::cout << "Экипировано " << weaponItem->getName() << "! Бонус к атаке: " 
                      << weaponItem->getDamageBonus() << std::endl;
        }
    }
    
    void displayCharacterInfo() const {
        std::cout << "Имя: " << characterName << ", Здоровье: " << currentHealth << "/" << maximumHealth
                  << ", Атака: " << attackStat << ", Защита: " << defenseStat
                  << ", Уровень: " << characterLevel << ", Опыт: " << experiencePoints << "/100" << std::endl;
    }
    
    void saveCharacterProgress(const std::string& saveFile) const {
        std::ofstream outputFile(saveFile);
        if (!outputFile) {
            throw GameWorldException("Ошибка сохранения персонажа");
        }
        
        outputFile << characterName << "\n";
        outputFile << currentHealth << "\n";
        outputFile << maximumHealth << "\n";
        outputFile << attackStat << "\n";
        outputFile << defenseStat << "\n";
        outputFile << characterLevel << "\n";
        outputFile << experiencePoints << "\n";
        
        characterInventory.saveInventory(saveFile + "_инвентарь");
    }
    
    void loadCharacterProgress(const std::string& saveFile) {
        std::ifstream inputFile(saveFile);
        if (!inputFile) {
            throw GameWorldException("Ошибка загрузки персонажа");
        }
        
        std::getline(inputFile, characterName);
        
        std::string dataLine;
        std::getline(inputFile, dataLine); currentHealth = std::stoi(dataLine);
        std::getline(inputFile, dataLine); maximumHealth = std::stoi(dataLine);
        std::getline(inputFile, dataLine); attackStat = std::stoi(dataLine);
        std::getline(inputFile, dataLine); defenseStat = std::stoi(dataLine);
        std::getline(inputFile, dataLine); characterLevel = std::stoi(dataLine);
        std::getline(inputFile, dataLine); experiencePoints = std::stoi(dataLine);
        
        characterInventory.loadInventory(saveFile + "_инвентарь");
    }
    
    void addItemToInventory(std::unique_ptr<GameItem> newItem) {
        characterInventory.addItem(std::move(newItem));
    }
    
    void showCharacterInventory() const {
        characterInventory.displayContents();
    }
    
    std::string getName() const { return characterName; }
    int getHealth() const { return currentHealth; }
    int getMaxHealth() const { return maximumHealth; }
    int getAttack() const { return attackStat; }
    int getDefense() const { return defenseStat; }
    int getLevel() const { return characterLevel; }
    int getExperience() const { return experiencePoints; }
};

class AdventureGame {
private:
    GameCharacter mainCharacter;
    EventLogger<std::string> gameLogger;
    
public:
    AdventureGame(const std::string& playerName) 
        : mainCharacter(playerName, 100, 10, 5), gameLogger("журнал_игры.txt") {
        gameLogger.recordEvent("Начало игры. Персонаж: " + playerName);
    }
    
    void initiateCombat(GameCreature& enemy) {
        gameLogger.recordEvent("Битва между " + mainCharacter.getName() + " и " + enemy.getName());
        
        std::cout << "\n=== НАЧАЛО БИТВЫ ===\n";
        mainCharacter.displayCharacterInfo();
        enemy.displayCreatureInfo();
        std::cout << "====================\n";
        
        try {
            while (mainCharacter.getHealth() > 0 && enemy.isAlive()) {

                std::cout << "\nВаш ход:\n";
                std::cout << "1. Атаковать\n";
                std::cout << "2. Использовать предмет\n";
                std::cout << "Выберите действие: ";
                
                int playerChoice;
                std::cin >> playerChoice;
                
                if (playerChoice == 1) {
                    mainCharacter.attackTarget(enemy);
                    gameLogger.recordEvent(mainCharacter.getName() + " атакует " + enemy.getName());
                } else if (playerChoice == 2) {
                    mainCharacter.showCharacterInventory();
                    std::cout << "Введите название предмета: ";
                    std::string selectedItem;
                    std::cin.ignore();
                    std::getline(std::cin, selectedItem);
                    
                    try {
                        mainCharacter.useInventoryItem(selectedItem);
                        gameLogger.recordEvent(mainCharacter.getName() + " использует: " + selectedItem);
                    } catch (const GameWorldException& error) {
                        std::cerr << "Ошибка: " << error.what() << std::endl;
                        continue;
                    }
                } else {
                    std::cout << "Неверный выбор!" << std::endl;
                    continue;
                }
                
                if (!enemy.isAlive()) {
                    int experienceReward = enemy.getAttack() * 5;
                    mainCharacter.gainExperience(experienceReward);
                    std::cout << enemy.getName() << " побежден! Получено " << experienceReward << " опыта.\n";
                    gameLogger.recordEvent(mainCharacter.getName() + " побеждает " + enemy.getName() + 
                               " и получает " + std::to_string(experienceReward) + " опыта");
                    return;
                }
                
                std::cout << "\nХод " << enemy.getName() << ":\n";
                enemy.performAttack(mainCharacter);
                gameLogger.recordEvent(enemy.getName() + " атакует " + mainCharacter.getName());
                
                std::cout << "\nТекущее состояние:\n";
                mainCharacter.displayCharacterInfo();
                enemy.displayCreatureInfo();
                std::cout << "====================\n";
            }
        } catch (const GameWorldException& error) {
            gameLogger.recordEvent("Конец битвы: " + std::string(error.what()));
            throw;
        }
    }
    
    void saveGameState() {
        try {
            mainCharacter.saveCharacterProgress("сохранение.txt");
            gameLogger.recordEvent("Игра сохранена");
            std::cout << "Игра сохранена!\n";
        } catch (const GameWorldException& error) {
            gameLogger.recordEvent("Ошибка сохранения: " + std::string(error.what()));
            std::cerr << "Ошибка сохранения: " << error.what() << std::endl;
        }
    }
    
    void loadGameState() {
        try {
            mainCharacter.loadCharacterProgress("сохранение.txt");
            gameLogger.recordEvent("Игра загружена");
            std::cout << "Игра загружена!\n";
        } catch (const GameWorldException& error) {
            gameLogger.recordEvent("Ошибка загрузки: " + std::string(error.what()));
            std::cerr << "Ошибка загрузки: " << error.what() << std::endl;
        }
    }
    
    void addItemToCharacter(std::unique_ptr<GameItem> newItem) {
        mainCharacter.addItemToInventory(std::move(newItem));
    }
    
    void showCharacterStatus() const {
        mainCharacter.displayCharacterInfo();
    }
    
    void showCharacterItems() const {
        mainCharacter.showCharacterInventory();
    }
};

int main() {
    try {
        std::cout << "Введите имя вашего персонажа: ";
        std::string playerName;
        std::getline(std::cin, playerName);
        
        AdventureGame gameSession(playerName);
        
        gameSession.addItemToCharacter(std::make_unique<CombatGear>("Стальной меч", "Острый стальной меч", 5));
        gameSession.addItemToCharacter(std::make_unique<HealingItem>("Целебное зелье", "Восстанавливает здоровье", 30));
        
        while (true) {
            std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
            std::cout << "1. Сразиться с лесным гоблином\n";
            std::cout << "2. Сразиться с древним драконом\n";
            std::cout << "3. Сразиться с нежитью-воином\n";
            std::cout << "4. Показать информацию о персонаже\n";
            std::cout << "5. Показать инвентарь\n";
            std::cout << "6. Сохранить игру\n";
            std::cout << "7. Загрузить игру\n";
            std::cout << "8. Выйти\n";
            std::cout << "Выберите действие: ";
            
            int menuChoice;
            std::cin >> menuChoice;
            
            try {
                switch (menuChoice) {
                    case 1: {
                        ForestGoblin goblinEnemy;
                        gameSession.initiateCombat(goblinEnemy);
                        break;
                    }
                    case 2: {
                        AncientDragon dragonEnemy;
                        gameSession.initiateCombat(dragonEnemy);
                        break;
                    }
                    case 3: {
                        UndeadWarrior undeadEnemy;
                        gameSession.initiateCombat(undeadEnemy);
                        break;
                    }
                    case 4:
                        gameSession.showCharacterStatus();
                        break;
                    case 5:
                        gameSession.showCharacterItems();
                        break;
                    case 6:
                        gameSession.saveGameState();
                        break;
                    case 7:
                        gameSession.loadGameState();
                        break;
                    case 8:
                        return 0;
                    default:
                        std::cout << "Неверный выбор!\n";
                }
            } catch (const GameWorldException& error) {
                std::cerr << "Игра окончена: " << error.what() << std::endl;
                return 1;
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "Критическая ошибка: " << error.what() << std::endl;
        return 1;
    }
    
    return 0;
}
