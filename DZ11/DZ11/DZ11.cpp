#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
using namespace std;

void divide(int dividend, int divisor) {
    if (divisor == 0) {
        throw invalid_argument("Деление на ноль запрещено.");
    }

    try {
        double quotient = static_cast<double>(dividend) / divisor;
        cout << "Частное чисел " << dividend << " и " << divisor << " равно " << quotient << endl;
    }
    catch (const std::invalid_argument& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
    catch (const std::runtime_error& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void openFile(const string& filename) {
    try {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Не удалось открыть файл.");
        }
        cout << "Файл успешно открыт." << endl;
    }
    catch (const runtime_error& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void printElement(int arr[], int size, int index) {
    try {
        if (index < 0 || index >= size) {
            throw runtime_error("Индекс выходит за границы массива.");
        }
        cout << "Элемент массива по индексу " << index << " равен " << arr[index] << endl;
    }
    catch (const runtime_error& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

class InvalidPasswordException : public runtime_error {
public:
    InvalidPasswordException(const string& message) : runtime_error(message) {}
};
class UserNotFoundException : public runtime_error {
public:
    UserNotFoundException(const string& message) : runtime_error(message) {}
};
class User {
public:
    int ID;
    string username;
    string password;

    User(int id, const string& name, const string& pass) : ID(id), username(name), password(pass) {
        validatePassword();
    }

private:
    void validatePassword() {
        if (password.size() < 8) {
            throw InvalidPasswordException("Пароль должен быть не менее 8 символов.");
        }

        bool hasUpper = false;
        bool hasLower = false;
        bool hasDigit = false;

        for (char c : password) {
            if (isupper(c)) {
                hasUpper = true;
            }
            else if (islower(c)) {
                hasLower = true;
            }
            else if (isdigit(c)) {
                hasDigit = true;
            }
        }

        if (!hasUpper || !hasLower || !hasDigit) {
            throw InvalidPasswordException("Пароль должен содержать хотя бы одну заглавную букву, одну строчную букву и одну цифру.");
        }
    }
};
class UserManager {
private:
    vector<User> users;

public:
    void RegisterUser(const User& user) {
        users.push_back(user);
    }

    bool UserExists(int id) const {
        for (const User& user : users) {
            if (user.ID == id) {
                return true;
            }
        }
        return false;
    }

    void FindUserById(int id) const {
        for (const User& user : users) {
            if (user.ID == id) {
                cout << "Найден пользователь: " << user.username << ", ID: " << user.ID << endl;
                return;
            }
        }
        throw UserNotFoundException("Пользователь с указанным ID не найден.");
    }

    void ChangePassword(int id, const string& oldPassword, const string& newPassword) {
        bool isUserFound = false;
        for (User& user : users) {
            if (user.ID == id) {
                isUserFound = true;
                if (user.password == oldPassword) {
                    if (newPassword.size() >= 8 ) {
                        cout << "Пароль пользователя с ID: " << id << " был сменен на пароль " << newPassword << endl;
                        user.password = newPassword;
                    }
                    else {
                        throw InvalidPasswordException("Новый пароль должен быть не менее 8 символов и содержать цифру.");
                    }
                }
                else {
                    throw InvalidPasswordException("Неправильно введен старый пароль.");
                }
            }
        }
        if (!isUserFound) {
            throw UserNotFoundException("Пользователь с указанным ID не найден.");
        }
    }
};

class EmptyCartException : public exception{
    const char* what() const throw(){
        return "Пустая корзина.";
    }
};

class OutOfStockException : public exception {
public:
    const char* what() const throw() {
        return "Столько товара нет.";
    }
};

class Product{
protected:
    string name;
    double price;
    int stock;
public:
    Product(string name, double price, int stock) : name(name), price(price), stock(stock) {}
    int quantity;
    virtual void addStock(int quantity){
        stock += quantity;
    }

    virtual void reduceStock(int quantity){
        if (quantity > stock){
            throw OutOfStockException();
        }
        stock -= quantity;
    }

    virtual double getPrice() const{
        return price;
    }

    virtual void displayPrice() const{
        cout << name << ": " << price << " руб." << endl;
    }
};

class Technic : public Product{
public:
    Technic(string name, double price, int stock) : Product(name, price, stock) {}

    double getPrice() const override{
        return price / 2;
    }

    void displayPrice() const override{
        cout << name << " (техника): " << price / 2<< " руб." << endl;
    }

    void addStock(int quantity) override {
        stock += quantity;
    }

    void reduceStock(int quantity) override {
        if (quantity > stock) {
            throw OutOfStockException();
        }
        stock -= quantity;
    }
};

class Food : public Product{
public:
    Food(string name, double price, int stock) : Product(name, price, stock) {}
    double getPrice() const override {
        return price;
    }

    void displayPrice() const override {
        cout << name << " (еда): " << price * quantity << " руб." << endl;
    }

    void addStock(int quantity) override{
        stock += quantity * 10;
    }

    void reduceStock(int quantity) override{
        if (quantity*10 > stock){
            throw OutOfStockException();
        }
        stock -= quantity * 10;
    }
};

class ShoppingCart{
private:
    vector<Product*> products;

public:
    void addProduct(Product* product, int quantity){
        try{
            product->quantity = quantity;
            product->reduceStock(quantity);
            products.push_back(product);
        }
        catch (const OutOfStockException& e){
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void checkout(){
        if (products.empty()){
            throw EmptyCartException();
        }

        double total = 0;
        for (auto& product : products){
            total += product->getPrice() * product->quantity;
            product->displayPrice();
        }
        cout << "Итого: " << total <<" руб." << endl;
        products.clear();
    }
};

int main() {
    setlocale(LC_ALL, "");

    ////Task 1
    //try {
    //    int num1, num2;
    //    cout << "Задание 1\nВведите первое число: ";
    //    cin >> num1;
    //    cout << "Введите второе число: ";
    //    cin >> num2;
    //    divide(num1, num2);
    //}
    //catch (const invalid_argument& e) {
    //    cerr << "Ошибка: " << e.what() << endl;
    //}
    //catch (const runtime_error& e) {
    //    cerr << "Ошибка: " << e.what() << endl;
    //}

    ////Task 2
    //try {
    //    string filename;
    //    cout <<endl << "Задание 2\nВведите имя файла: ";
    //    cin >> filename;
    //    openFile(filename);
    //}
    //catch (const runtime_error& e) {
    //    cerr << "Ошибка: " << e.what() << endl;
    //}

    ////Task 3
    //try {
    //    int arr[] = { 1, 2, 3, 4, 5 };
    //    int size = sizeof(arr) / sizeof(arr[0]);
    //    int index;
    //    cout << "\nЗадание 3\nВведите индекс элемента массива: ";
    //    cin >> index;
    //    printElement(arr, size, index);
    //}
    //catch (const runtime_error& e) {
    //    cerr << "Ошибка: " << e.what() << endl;
    //}

    ////Task 4
    //try {
    //    string name, password;
    //    int ID;
    //    cout << "\nЗадание 4\n";
    //    cout << "Введите имя пользователя: ";
    //    cin >> name;
    //    cout << "Введите пароль: ";
    //    cin >> password;
    //    cout << "Введите ID, который будет установлен вам: ";
    //    cin >> ID;
    //    User user1(ID, name, password);
    //    cout << "Пользователь создан." << endl;
    //}
    //catch (const InvalidPasswordException& e) {
    //    cerr << "Ошибка: " << e.what() << endl;
    //}
    //catch (const runtime_error& e) {
    //    cerr << "Ошибка: " << e.what() << endl;
    //}

    ////Task 5 
    //UserManager userManager;
    //userManager.RegisterUser(User(1, "Сергей", "Qweasdzxc123"));
    //userManager.RegisterUser(User(2, "Максим", "Maksim123"));
    //try {
    //    cout << "\nЗадание 5\n";
    //    if (userManager.UserExists(1)) {
    //        cout << "Пользователь с ID 1 существует." << endl;
    //    }
    //    else {
    //        cout << "Пользователь с ID 1 не существует." << endl;
    //    }
    //    int temp;
    //    cout << "Введите ID пользователя которого ищем: ";
    //    cin >> temp;
    //    userManager.FindUserById(temp);
    //}
    //catch (const UserNotFoundException& e) {
    //    cerr << "Ошибка: " << e.what() << endl;
    //}
    //catch (const runtime_error& e) {
    //    cerr << "Ошибка: " << e.what() << endl;
    //}

    ////Task 6
    //UserManager userManager;
    //userManager.RegisterUser(User(1, "Сергей", "Qweasdzxc123"));
    //userManager.RegisterUser(User(2, "Максим", "Maksim123"));
    //try {
    //    cout << "\nЗадание 6\n";
    //    int temp;
    //    string old, nEw;
    //    cout << "Введите Id пользователя пароль которого вы хотите поменять: ";
    //    cin >> temp;
    //    cout << "Введите старый пароль: ";
    //    cin >> old;
    //    cout << "Введите новый пароль: ";
    //    cin >> nEw;
    //    userManager.ChangePassword(temp, old, nEw);
    //}
    //catch (const UserNotFoundException& e) {
    //    cerr << "Ошибка: " << e.what() << endl;
    //}
    //catch (const InvalidPasswordException& e) {
    //    cerr << "Ошибка: " << e.what() << endl;
    //}
    //catch (const runtime_error& e) {
    //    cerr << "Ошибка: " << e.what() << endl;
    //}

    //Task 7
    Technic техника("Дрель ваще чумовой инструмент",20000, 5);
    Food еда("Бананы", 100, 50);

    ShoppingCart корзина;
    корзина.addProduct(&техника, 2);
    корзина.addProduct(&еда, 5);
    корзина.checkout();

    return 0;
}
