#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Базовый класс для игровых объектов
class GameObject {
public:
    std::string title;
    int hitPoints;

    GameObject(const std::string& title = "", int hitPoints = 0) : title(title), hitPoints(hitPoints) {}

    // Метод для записи данных в файл
    void store(std::ofstream& file) const {
        file << title << " " << hitPoints << "\n";
    }

    // Метод для чтения данных из файла
    void retrieve(std::ifstream& file) {
        file >> title >> hitPoints;
    }
};

// Менеджер игровых объектов
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

// Сохранение данных в файл
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
    handler.objects.clear(); // Очищаем текущие данные
    GameObject obj;
    while (file >> obj.title >> obj.hitPoints) {
        handler.objects.push_back(obj); // Загружаем объекты
    }
}

int main() {
    try {
        ObjectHandler handler;
        handler.appendObject(GameObject("Воин", 120));
        handler.appendObject(GameObject("Маг", 70));

        // Сохраняем в файл
        storeData(handler, "data.txt");
        std::cout << "Данные сохранены!\n";

        // Загружаем из файла
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
