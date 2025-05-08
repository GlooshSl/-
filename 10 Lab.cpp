#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <typeinfo>

class AccessViolationError : public std::runtime_error {
public:
    AccessViolationError(const std::string& msg) : std::runtime_error(msg) {}
};

class DataValidationError : public std::runtime_error {
public:
    DataValidationError(const std::string& msg) : std::runtime_error(msg) {}
};

class UniversityMember {
protected:
    std::string fullName;
    int memberId;
    int clearanceLevel;

    void validate() const {
        if (fullName.empty()) {
            throw DataValidationError("ФИО обязательно для заполнения");
        }
        if (memberId <= 0) {
            throw DataValidationError("Идентификатор должен быть положительным");
        }
        if (clearanceLevel < 1 || clearanceLevel > 3) {
            throw DataValidationError("Уровень доступа от 1 до 3");
        }
    }

public:
    UniversityMember(const std::string& name, int id, int level) 
        : fullName(name), memberId(id), clearanceLevel(level) {
        validate();
    }

    virtual ~UniversityMember() {}

    std::string getFullName() const { return fullName; }
    int getMemberId() const { return memberId; }
    int getClearanceLevel() const { return clearanceLevel; }

    void setFullName(const std::string& name) {
        if (name.empty()) throw DataValidationError("ФИО обязательно для заполнения");
        fullName = name;
    }

    void setMemberId(int id) {
        if (id <= 0) throw DataValidationError("Идентификатор должен быть положительным");
        memberId = id;
    }

    void setClearanceLevel(int level) {
        if (level < 1 || level > 3) throw DataValidationError("Уровень доступа от 1 до 3");
        clearanceLevel = level;
    }

    virtual void showDetails() const {
        std::cout << "ID: " << memberId << ", ФИО: " << fullName 
                  << ", Уровень доступа: " << clearanceLevel;
    }

    virtual void save(std::ofstream& file) const {
        file << typeid(*this).name() << "\n" << fullName << "\n" << memberId << "\n" << clearanceLevel << "\n";
    }

    virtual void load(std::ifstream& file) {
        std::getline(file, fullName);
        file >> memberId >> clearanceLevel;
        file.ignore();
        validate();
    }
};

class Student : public UniversityMember {
private:
    std::string studyGroup;

public:
    Student(const std::string& name, int id, const std::string& group) 
        : UniversityMember(name, id, 1), studyGroup(group) {
        if (studyGroup.empty()) throw DataValidationError("Группа обязательна");
    }

    void showDetails() const override {
        UniversityMember::showDetails();
        std::cout << ", Статус: Студент, Группа: " << studyGroup << std::endl;
    }

    std::string getStudyGroup() const { return studyGroup; }
    void setStudyGroup(const std::string& group) {
        if (group.empty()) throw DataValidationError("Группа обязательна");
        studyGroup = group;
    }

    void save(std::ofstream& file) const override {
        UniversityMember::save(file);
        file << studyGroup << "\n";
    }

    void load(std::ifstream& file) override {
        UniversityMember::load(file);
        std::getline(file, studyGroup);
        if (studyGroup.empty()) throw DataValidationError("Группа обязательна");
    }
};

class Professor : public UniversityMember {
private:
    std::string facultyDepartment;

public:
    Professor(const std::string& name, int id, const std::string& department) 
        : UniversityMember(name, id, 2), facultyDepartment(department) {
        if (facultyDepartment.empty()) throw DataValidationError("Кафедра обязательна");
    }

    void showDetails() const override {
        UniversityMember::showDetails();
        std::cout << ", Статус: Преподаватель, Кафедра: " << facultyDepartment << std::endl;
    }

    std::string getFacultyDepartment() const { return facultyDepartment; }
    void setFacultyDepartment(const std::string& department) {
        if (department.empty()) throw DataValidationError("Кафедра обязательна");
        facultyDepartment = department;
    }

    void save(std::ofstream& file) const override {
        UniversityMember::save(file);
        file << facultyDepartment << "\n";
    }

    void load(std::ifstream& file) override {
        UniversityMember::load(file);
        std::getline(file, facultyDepartment);
        if (facultyDepartment.empty()) throw DataValidationError("Кафедра обязательна");
    }
};

class UniversityStaff : public UniversityMember {
private:
    std::string jobTitle;

public:
    UniversityStaff(const std::string& name, int id, const std::string& title) 
        : UniversityMember(name, id, 3), jobTitle(title) {
        if (jobTitle.empty()) throw DataValidationError("Должность обязательна");
    }

    void showDetails() const override {
        UniversityMember::showDetails();
        std::cout << ", Статус: Персонал, Должность: " << jobTitle << std::endl;
    }

    std::string getJobTitle() const { return jobTitle; }
    void setJobTitle(const std::string& title) {
        if (title.empty()) throw DataValidationError("Должность обязательна");
        jobTitle = title;
    }

    void save(std::ofstream& file) const override {
        UniversityMember::save(file);
        file << jobTitle << "\n";
    }

    void load(std::ifstream& file) override {
        UniversityMember::load(file);
        std::getline(file, jobTitle);
        if (jobTitle.empty()) throw DataValidationError("Должность обязательна");
    }
};

class CampusFacility {
private:
    std::string facilityName;
    int minAccessLevel;

public:
    CampusFacility(const std::string& name, int level) 
        : facilityName(name), minAccessLevel(level) {
        if (facilityName.empty()) throw DataValidationError("Название обязательно");
        if (level < 1 || level > 3) throw DataValidationError("Уровень доступа от 1 до 3");
    }

    std::string getFacilityName() const { return facilityName; }
    int getMinAccessLevel() const { return minAccessLevel; }

    void setFacilityName(const std::string& name) {
        if (name.empty()) throw DataValidationError("Название обязательно");
        facilityName = name;
    }

    void setMinAccessLevel(int level) {
        if (level < 1 || level > 3) throw DataValidationError("Уровень доступа от 1 до 3");
        minAccessLevel = level;
    }

    void showInfo() const {
        std::cout << "Объект: " << facilityName 
                  << ", Минимальный уровень доступа: " << minAccessLevel << std::endl;
    }

    bool verifyAccess(const UniversityMember& member) const {
        return member.getClearanceLevel() >= minAccessLevel;
    }

    void save(std::ofstream& file) const {
        file << facilityName << "\n" << minAccessLevel << "\n";
    }

    void load(std::ifstream& file) {
        std::getline(file, facilityName);
        file >> minAccessLevel;
        file.ignore();
        if (facilityName.empty()) throw DataValidationError("Название обязательно");
        if (minAccessLevel < 1 || minAccessLevel > 3) 
            throw DataValidationError("Уровень доступа от 1 до 3");
    }
};

template<typename T>
class AccessManagementSystem {
private:
    std::vector<std::unique_ptr<UniversityMember>> members;
    std::vector<T> facilities;

public:
    void addMember(std::unique_ptr<UniversityMember> member) {
        members.push_back(std::move(member));
    }

    void addFacility(const T& facility) {
        facilities.push_back(facility);
    }

    void listAllMembers() const {
        for (const auto& member : members) {
            member->showDetails();
        }
    }

    void listAllFacilities() const {
        for (const auto& facility : facilities) {
            facility.showInfo();
        }
    }

    bool verifyMemberAccess(int memberId, const std::string& facilityName) const {
        auto memberIt = find_if(members.begin(), members.end(), 
            [memberId](const auto& m) { return m->getMemberId() == memberId; });
        
        auto facilityIt = find_if(facilities.begin(), facilities.end(), 
            [facilityName](const T& f) { return f.getFacilityName() == facilityName; });

        if (memberIt == members.end()) {
            throw std::runtime_error("Член университета с ID " + std::to_string(memberId) + " не найден");
        }

        if (facilityIt == facilities.end()) {
            throw std::runtime_error("Объект " + facilityName + " не найден");
        }

        if (!facilityIt->verifyAccess(**memberIt)) {
            throw AccessViolationError("Доступ запрещен для " + (*memberIt)->getFullName() + 
                                   " к объекту " + facilityName);
        }

        return true;
    }

    void saveData(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) throw std::runtime_error("Ошибка открытия файла");

        file << members.size() << "\n";
        for (const auto& member : members) {
            member->save(file);
        }

        file << facilities.size() << "\n";
        for (const auto& facility : facilities) {
            facility.save(file);
        }
    }

    void loadData(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw std::runtime_error("Ошибка открытия файла");

        members.clear();
        facilities.clear();

        int memberCount;
        file >> memberCount;
        file.ignore();

        for (int i = 0; i < memberCount; ++i) {
            std::string type;
            std::getline(file, type);

            std::unique_ptr<UniversityMember> member;
            if (type == typeid(Student).name()) {
                member = std::make_unique<Student>("", 0, "");
            } else if (type == typeid(Professor).name()) {
                member = std::make_unique<Professor>("", 0, "");
            } else if (type == typeid(UniversityStaff).name()) {
                member = std::make_unique<UniversityStaff>("", 0, "");
            } else {
                throw std::runtime_error("Неизвестный тип члена университета");
            }

            member->load(file);
            members.push_back(std::move(member));
        }

        int facilityCount;
        file >> facilityCount;
        file.ignore();

        for (int i = 0; i < facilityCount; ++i) {
            T facility("", 0);
            facility.load(file);
            facilities.push_back(facility);
        }
    }

    void findMemberByName(const std::string& name) const {
        bool found = false;
        for (const auto& member : members) {
            if (member->getFullName() == name) {
                member->showDetails();
                found = true;
            }
        }
        if (!found) {
            std::cout << "Член университета " << name << " не найден" << std::endl;
        }
    }

    void findMemberById(int id) const {
        bool found = false;
        for (const auto& member : members) {
            if (member->getMemberId() == id) {
                member->showDetails();
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "Член университета с ID " << id << " не найден" << std::endl;
        }
    }

    void sortMembersByAccessLevel() {
        sort(members.begin(), members.end(), 
            [](const auto& a, const auto& b) {
                return a->getClearanceLevel() < b->getClearanceLevel();
            });
    }

    void sortMembersByName() {
        sort(members.begin(), members.end(), 
            [](const auto& a, const auto& b) {
                return a->getFullName() < b->getFullName();
            });
    }
};

std::unique_ptr<UniversityMember> createUniversityMember() {
    std::cout << "Тип члена университета:\n";
    std::cout << "1. Студент\n2. Преподаватель\n3. Персонал\nВыбор: ";

    int type;
    std::cin >> type;
    std::cin.ignore();

    std::string name;
    std::cout << "ФИО: ";
    std::getline(std::cin, name);

    int id;
    std::cout << "ID: ";
    std::cin >> id;
    std::cin.ignore();

    try {
        switch (type) {
            case 1: {
                std::string group;
                std::cout << "Группа: ";
                std::getline(std::cin, group);
                return std::make_unique<Student>(name, id, group);
            }
            case 2: {
                std::string department;
                std::cout << "Кафедра: ";
                std::getline(std::cin, department);
                return std::make_unique<Professor>(name, id, department);
            }
            case 3: {
                std::string position;
                std::cout << "Должность: ";
                std::getline(std::cin, position);
                return std::make_unique<UniversityStaff>(name, id, position);
            }
            default:
                throw DataValidationError("Неверный тип");
        }
    } catch (const DataValidationError& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return nullptr;
    }
}

CampusFacility createCampusFacility() {
    std::string name;
    std::cout << "Название объекта: ";
    std::getline(std::cin, name);

    int level;
    std::cout << "Уровень доступа (1-3): ";
    std::cin >> level;
    std::cin.ignore();

    try {
        return CampusFacility(name, level);
    } catch (const DataValidationError& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        throw;
    }
}

void displayMainMenu(AccessManagementSystem<CampusFacility>& system) {
    while (true) {
        std::cout << "\n=== Управление доступом ===\n";
        std::cout << "1. Добавить члена университета\n";
        std::cout << "2. Добавить объект\n";
        std::cout << "3. Показать всех членов\n";
        std::cout << "4. Показать все объекты\n";
        std::cout << "5. Проверить доступ\n";
        std::cout << "6. Найти по ФИО\n";
        std::cout << "7. Найти по ID\n";
        std::cout << "8. Сортировать по уровню доступа\n";
        std::cout << "9. Сортировать по ФИО\n";
        std::cout << "10. Сохранить данные\n";
        std::cout << "11. Загрузить данные\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        try {
            switch (choice) {
                case 1: {
                    auto member = createUniversityMember();
                    if (member) {
                        system.addMember(std::move(member));
                        std::cout << "Добавлено\n";
                    }
                    break;
                }
                case 2: {
                    try {
                        CampusFacility facility = createCampusFacility();
                        system.addFacility(facility);
                        std::cout << "Добавлено\n";
                    } catch (...) {}
                    break;
                }
                case 3:
                    system.listAllMembers();
                    break;
                case 4:
                    system.listAllFacilities();
                    break;
                case 5: {
                    int id;
                    std::cout << "ID члена: ";
                    std::cin >> id;
                    std::cin.ignore();

                    std::string facility;
                    std::cout << "Объект: ";
                    std::getline(std::cin, facility);

                    try {
                        if (system.verifyMemberAccess(id, facility)) {
                            std::cout << "Доступ разрешен\n";
                        }
                    } catch (const AccessViolationError& e) {
                        std::cerr << "Отказано: " << e.what() << std::endl;
                    } catch (const std::runtime_error& e) {
                        std::cerr << "Ошибка: " << e.what() << std::endl;
                    }
                    break;
                }
                case 6: {
                    std::string name;
                    std::cout << "ФИО: ";
                    std::getline(std::cin, name);
                    system.findMemberByName(name);
                    break;
                }
                case 7: {
                    int id;
                    std::cout << "ID: ";
                    std::cin >> id;
                    std::cin.ignore();
                    system.findMemberById(id);
                    break;
                }
                case 8:
                    system.sortMembersByAccessLevel();
                    std::cout << "Отсортировано\n";
                    break;
                case 9:
                    system.sortMembersByName();
                    std::cout << "Отсортировано\n";
                    break;
                case 10: {
                    std::string filename;
                    std::cout << "Файл: ";
                    std::getline(std::cin, filename);
                    system.saveData(filename);
                    std::cout << "Сохранено\n";
                    break;
                }
                case 11: {
                    std::string filename;
                    std::cout << "Файл: ";
                    std::getline(std::cin, filename);
                    system.loadData(filename);
                    std::cout << "Загружено\n";
                    break;
                }
                case 0:
                    return;
                default:
                    std::cout << "Неверный выбор\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }
}

int main() {
    AccessManagementSystem<CampusFacility> system;

    try {
        system.addMember(std::make_unique<Student>("Иванов Иван", 101, "ИТ-101"));
        system.addMember(std::make_unique<Professor>("Петров Петр", 201, "Информатика"));
        system.addMember(std::make_unique<UniversityStaff>("Сидорова Мария", 301, "Администратор"));

        system.addFacility(CampusFacility("Аудитория 101", 1));
        system.addFacility(CampusFacility("Лаборатория", 2));
        system.addFacility(CampusFacility("Серверная", 3));
    } catch (const std::exception& e) {
        std::cerr << "Ошибка инициализации: " << e.what() << std::endl;
    }

    displayMainMenu(system);

    return 0;
}
