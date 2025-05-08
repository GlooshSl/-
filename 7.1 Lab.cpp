#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class GameObject {
public:
    std::string title;
    int hitPoints;

    GameObject(const std::string& title = "", int hitPoints = 0) : title(title), hitPoints(hitPoints) {}

    void store(std::ofstream& file) const {
        file << title << " " << hitPoints << "\n";
    }

    void retrieve(std::ifstream& file) {
        file >> title >> hitPoints;
    }
};

class ObjectHandler {
public:
    std::vector<GameObject> objects;

    void appendObject(const GameObject& obj) {
        objects.push_back(obj);
    }

    void listAll() const {
        for (const auto& obj : objects) {
            std::cout << "Название: " << obj.title 
                      << ", Очки жизни: " << obj.hitPoints << "\n";
        }
    }
};

void storeData(const ObjectHandler& handler, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Ошибка открытия файла для записи.");
    }
    for (const auto& obj : handler.objects) {
        obj.store(file); // Сохраняем каждый объект
    }
}

// Загрузка данных из файла
void loadData(ObjectHandler& handler, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Ошибка открытия файла для чтения.");
    }
    handler.objects.clear(); 
    GameObject obj;
    while (file >> obj.title >> obj.hitPoints) {
        handler.objects.push_back(obj);
    }
}

int main() {
    try {
        ObjectHandler handler;
        handler.appendObject(GameObject("Воин", 120));
        handler.appendObject(GameObject("Маг", 70));

        storeData(handler, "data.txt");
        std::cout << "Данные сохранены!\n";

        ObjectHandler loadedHandler;
        loadData(loadedHandler, "data.txt");
        std::cout << "Загруженные объекты:\n";
        loadedHandler.listAll();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
    return 0;
}
