#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <typeinfo>
#include <locale.h> 
using namespace std;

class AccessViolationError : public runtime_error {
public:
    AccessViolationError(const string& msg) : runtime_error(msg) {}
};

class DataValidationError : public runtime_error {
public:
    DataValidationError(const string& msg) : runtime_error(msg) {}
};

class UniversityMember {
protected:
    string fullName;
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
    UniversityMember(const string& name, int id, int level)
        : fullName(name), memberId(id), clearanceLevel(level) {
        validate();
    }

    virtual ~UniversityMember() {}

    string getFullName() const { return fullName; }
    int getMemberId() const { return memberId; }
    int getClearanceLevel() const { return clearanceLevel; }

    void setFullName(const string& name) {
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
        cout << "ID: " << memberId << ", ФИО: " << fullName
            << ", Уровень доступа: " << clearanceLevel;
    }

    virtual void save(ofstream& file) const {
        file << typeid(*this).name() << "\n" << fullName << "\n" << memberId << "\n" << clearanceLevel << "\n";
    }

    virtual void load(ifstream& file) {
        getline(file, fullName);
        file >> memberId >> clearanceLevel;
        file.ignore();
        validate();
    }
};

class Student : public UniversityMember {
private:
    string studyGroup;

public:
    Student(const string& name, int id, const string& group)
        : UniversityMember(name, id, 1), studyGroup(group) {
        if (studyGroup.empty()) throw DataValidationError("Группа обязательна");
    }

    void showDetails() const override {
        UniversityMember::showDetails();
        cout << ", Статус: Студент, Группа: " << studyGroup << endl;
    }

    string getStudyGroup() const { return studyGroup; }
    void setStudyGroup(const string& group) {
        if (group.empty()) throw DataValidationError("Группа обязательна");
        studyGroup = group;
    }

    void save(ofstream& file) const override {
        UniversityMember::save(file);
        file << studyGroup << "\n";
    }

    void load(ifstream& file) override {
        UniversityMember::load(file);
        std::getline(file, studyGroup);
        if (studyGroup.empty()) throw DataValidationError("Группа обязательна");
    }
};

class Professor : public UniversityMember {
private:
    string facultyDepartment;

public:
    Professor(const string& name, int id, const string& department)
        : UniversityMember(name, id, 2), facultyDepartment(department) {
        if (facultyDepartment.empty()) throw DataValidationError("Кафедра обязательна");
    }

    void showDetails() const override {
        UniversityMember::showDetails();
        cout << ", Статус: Преподаватель, Кафедра: " << facultyDepartment << endl;
    }

    string getFacultyDepartment() const { return facultyDepartment; }
    void setFacultyDepartment(const string& department) {
        if (department.empty()) throw DataValidationError("Кафедра обязательна");
        facultyDepartment = department;
    }

    void save(ofstream& file) const override {
        UniversityMember::save(file);
        file << facultyDepartment << "\n";
    }

    void load(ifstream& file) override {
        UniversityMember::load(file);
        getline(file, facultyDepartment);
        if (facultyDepartment.empty()) throw DataValidationError("Кафедра обязательна");
    }
};

class UniversityStaff : public UniversityMember {
private:
    string jobTitle;

public:
    UniversityStaff(const string& name, int id, const string& title)
        : UniversityMember(name, id, 3), jobTitle(title) {
        if (jobTitle.empty()) throw DataValidationError("Должность обязательна");
    }

    void showDetails() const override {
        UniversityMember::showDetails();
        cout << ", Статус: Персонал, Должность: " << jobTitle << endl;
    }

    string getJobTitle() const { return jobTitle; }
    void setJobTitle(const string& title) {
        if (title.empty()) throw DataValidationError("Должность обязательна");
        jobTitle = title;
    }

    void save(ofstream& file) const override {
        UniversityMember::save(file);
        file << jobTitle << "\n";
    }

    void load(ifstream& file) override {
        UniversityMember::load(file);
        getline(file, jobTitle);
        if (jobTitle.empty()) throw DataValidationError("Должность обязательна");
    }
};

class CampusFacility {
private:
    string facilityName;
    int minAccessLevel;

public:
    CampusFacility(const string& name, int level)
        : facilityName(name), minAccessLevel(level) {
        if (facilityName.empty()) throw DataValidationError("Название обязательно");
        if (level < 1 || level > 3) throw DataValidationError("Уровень доступа от 1 до 3");
    }

    string getFacilityName() const { return facilityName; }
    int getMinAccessLevel() const { return minAccessLevel; }

    void setFacilityName(const string& name) {
        if (name.empty()) throw DataValidationError("Название обязательно");
        facilityName = name;
    }

    void setMinAccessLevel(int level) {
        if (level < 1 || level > 3) throw DataValidationError("Уровень доступа от 1 до 3");
        minAccessLevel = level;
    }

    void showInfo() const {
        cout << "Объект: " << facilityName
            << ", Минимальный уровень доступа: " << minAccessLevel << endl;
    }

    bool verifyAccess(const UniversityMember& member) const {
        return member.getClearanceLevel() >= minAccessLevel;
    }

    void save(ofstream& file) const {
        file << facilityName << "\n" << minAccessLevel << "\n";
    }

    void load(ifstream& file) {
        getline(file, facilityName);
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
    vector<unique_ptr<UniversityMember>> members;
    vector<T> facilities;

public:
    void addMember(unique_ptr<UniversityMember> member) {
        members.push_back()move(member);
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

    bool verifyMemberAccess(int memberId, const string& facilityName) const {
        auto memberIt = find_if(members.begin(), members.end(),
            [memberId](const auto& m) { return m->getMemberId() == memberId; });

        auto facilityIt = find_if(facilities.begin(), facilities.end(),
            [facilityName](const T& f) { return f.getFacilityName() == facilityName; });

        if (memberIt == members.end()) {
            throw runtime_error("Член университета с ID " + to_string(memberId) + " не найден");
        }

        if (facilityIt == facilities.end()) {
            throw runtime_error("Объект " + facilityName + " не найден");
        }

        if (!facilityIt->verifyAccess(**memberIt)) {
            throw AccessViolationError("Доступ запрещен для " + (*memberIt)->getFullName() +
                " к объекту " + facilityName);
        }

        return true;
    }

    void saveData(const string& filename) const {
        ofstream file(filename);
        if (!file) throw runtime_error("Ошибка открытия файла");

        file << members.size() << "\n";
        for (const auto& member : members) {
            member->save(file);
        }

        file << facilities.size() << "\n";
        for (const auto& facility : facilities) {
            facility.save(file);
        }
    }

    void loadData(const string& filename) {
        ifstream file(filename);
        if (!file) throw runtime_error("Ошибка открытия файла");

        members.clear();
        facilities.clear();

        int memberCount;
        file >> memberCount;
        file.ignore();

        for (int i = 0; i < memberCount; ++i) {
            string type;
            getline(file, type);

            unique_ptr<UniversityMember> member;
            if (type == typeid(Student).name()) {
                member = make_unique<Student>("", 0, "");
            }
            else if (type == typeid(Professor).name()) {
                member = make_unique<Professor>("", 0, "");
            }
            else if (type == typeid(UniversityStaff).name()) {
                member = make_unique<UniversityStaff>("", 0, "");
            }
            else {
                throw runtime_error("Неизвестный тип члена университета");
            }

            member->load(file);
            members.push_back(move(member));
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

    void findMemberByName(const string& name) const {
        bool found = false;
        for (const auto& member : members) {
            if (member->getFullName() == name) {
                member->showDetails();
                found = true;
            }
        }
        if (!found) {
            cout << "Член университета " << name << " не найден" << endl;
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
            cout << "Член университета с ID " << id << " не найден" << endl;
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

unique_ptr<UniversityMember> createUniversityMember() {
    cout << "Тип члена университета:\n";
    cout << "1. Студент\n2. Преподаватель\n3. Персонал\nВыбор: ";

    int type;
    cin >> type;
    cin.ignore();

    string name;
    cout << "ФИО: ";
    getline(cin, name);

    int id;
    cout << "ID: ";
    cin >> id;
    cin.ignore();

    try {
        switch (type) {
        case 1: {
            string group;
            cout << "Группа: ";
            getline(cin, group);
            return make_unique<Student>(name, id, group);
        }
        case 2: {
            string department;
            cout << "Кафедра: ";
            getline(cin, department);
            return make_unique<Professor>(name, id, department);
        }
        case 3: {
            string position;
            cout << "Должность: ";
            getline(cin, position);
            return make_unique<UniversityStaff>(name, id, position);
        }
        default:
            throw DataValidationError("Неверный тип");
        }
    }
    catch (const DataValidationError& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return nullptr;
    }
}

CampusFacility createCampusFacility() {
    string name;
    cout << "Название объекта: ";
    getline(cin, name);

    int level;
    cout << "Уровень доступа (1-3): ";
    cin >> level;
    cin.ignore();

    try {
        return CampusFacility(name, level);
    }
    catch (const DataValidationError& e) {
        cerr << "Ошибка: " << e.what() << endl;
        throw;
    }
}

void displayMainMenu(AccessManagementSystem<CampusFacility>& system) {
    while (true) {
        cout << "\n=== Управление доступом ===\n";
        cout << "1. Добавить члена университета\n";
        cout << "2. Добавить объект\n";
        cout << "3. Показать всех членов\n";
        cout << "4. Показать все объекты\n";
        cout << "5. Проверить доступ\n";
        cout << "6. Найти по ФИО\n";
        cout << "7. Найти по ID\n";
        cout << "8. Сортировать по уровню доступа\n";
        cout << "9. Сортировать по ФИО\n";
        cout << "10. Сохранить данные\n";
        cout << "11. Загрузить данные\n";
        cout << "0. Выход\n";
        cout << "Выбор: ";

        int choice;
        cin >> choice;
        cin.ignore();

        try {
            switch (choice) {
            case 1: {
                auto member = createUniversityMember();
                if (member) {
                    system.addMember(move(member));
                    cout << "Добавлено\n";
                }
                break;
            }
            case 2: {
                try {
                    CampusFacility facility = createCampusFacility();
                    system.addFacility(facility);
                    cout << "Добавлено\n";
                }
                catch (...) {}
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
                cout << "ID члена: ";
                cin >> id;
                cin.ignore();

                string facility;
                cout << "Объект: ";
                getline(cin, facility);

                try {
                    if (system.verifyMemberAccess(id, facility)) {
                        cout << "Доступ разрешен\n";
                    }
                }
                catch (const AccessViolationError& e) {
                    cerr << "Отказано: " << e.what() << endl;
                }
                catch (const runtime_error& e) {
                    cerr << "Ошибка: " << e.what() << endl;
                }
                break;
            }
            case 6: {
                string name;
                cout << "ФИО: ";
                getline(cin, name);
                system.findMemberByName(name);
                break;
            }
            case 7: {
                int id;
                cout << "ID: ";
                cin >> id;
                cin.ignore();
                system.findMemberById(id);
                break;
            }
            case 8:
                system.sortMembersByAccessLevel();
                cout << "Отсортировано\n";
                break;
            case 9:
                system.sortMembersByName();
                cout << "Отсортировано\n";
                break;
            case 10: {
                string filename;
                cout << "Файл: ";
                getline(cin, filename);
                system.saveData(filename);
                cout << "Сохранено\n";
                break;
            }
            case 11: {
                string filename;
                cout << "Файл: ";
                getline(cin, filename);
                system.loadData(filename);
                cout << "Загружено\n";
                break;
            }
            case 0:
                return;
            default:
                cout << "Неверный выбор\n";
            }
        }
        catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }
}

int main() {

    setlocale(LC_ALL, "Russian");
    AccessManagementSystem<CampusFacility> system;

    try {
        system.addMember(make_unique<Student>("Иванов Иван", 101, "ИТ-101"));
        system.addMember(make_unique<Professor>("Петров Петр", 201, "Информатика"));
        system.addMember(make_unique<UniversityStaff>("Сидорова Мария", 301, "Администратор"));

        system.addFacility(CampusFacility("Аудитория 101", 1));
        system.addFacility(CampusFacility("Лаборатория", 2));
        system.addFacility(CampusFacility("Серверная", 3));
    }
    catch (const exception& e) {
        cerr << "Ошибка инициализации: " << e.what() << endl;
    }

    displayMainMenu(system);

    return 0;
}
