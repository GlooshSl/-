#include <iostream>
#include <string>

class UserProfile {
private:
    std::string fullName;
    int yearsOld;
    std::string emailAddress;
    std::string homeAddress;

public:
    std::string getFullName() const {
        return fullName;
    }

    int getAge() const {
        return yearsOld;
    }

    std::string getEmail() const {
        return emailAddress;
    }

    std::string getHomeAddress() const {
        return homeAddress;
    }

    void setFullName(const std::string& name) {
        if (!name.empty()) {
            fullName = name;
        } else {
            std::cerr << "Необходимо указать ФИО пользователя" << std::endl;
        }
    }

    void setAge(int age) {
        if (age >= 0 && age <= 120) {
            yearsOld = age;
        } else {
            std::cerr << "Допустимый возраст: от 0 до 120 лет" << std::endl;
        }
    }

    void setEmail(const std::string& email) {
        if (email.find('@') != std::string::npos) {
            emailAddress = email;
        } else {
            std::cerr << "Требуется электронная почта в правильном формате" << std::endl;
        }
    }

    void setHomeAddress(const std::string& address) {
        if (!address.empty()) {
            homeAddress = address;
        } else {
            std::cerr << "Адрес проживания обязателен для заполнения" << std::endl;
        }
    }

    void showProfile() const {
        std::cout << "\n=== Личный профиль ===" << std::endl;
        std::cout << "ФИО: " << fullName << std::endl;
        std::cout << "Возраст: " << yearsOld << " лет" << std::endl;
        std::cout << "Email: " << emailAddress << std::endl;
        std::cout << "Адрес: " << homeAddress << std::endl;
        std::cout << "=====================" << std::endl;
    }
};

int main() {
    UserProfile user;

    user.setFullName("Александр Петров");
    user.setAge(28);
    user.setEmail("alex.petrov@mailbox.com");
    user.setHomeAddress("г. Санкт-Петербург, Невский пр-т, д. 45, кв. 12");
    std::cout << "Краткая информация:" << std::endl;
    std::cout << "Пользователь: " << user.getFullName() << std::endl;
    std::cout << "Возраст: " << user.getAge() << " лет" << std::endl;
    user.showProfile()
    std::cout << "\nТестирование валидации данных:" << std::endl;
    user.setFullName("");
    user.setAge(150);
    user.setEmail("некорректный-email");
    user.setHomeAddress("");
    user.showProfile();

    return 0;
}
