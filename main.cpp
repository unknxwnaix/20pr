#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <algorithm>
#include <limits> // Добавить эту директиву для использования numeric_limits

using namespace std;

// Объявление классов
class Guest;
class Administrator;
class StockWorker;
class Accountant;

class Product {
private:
    string id;
    string name;
    double cost;

public:
    Product(const string& id, const string& name, double cost) : id(id), name(name), cost(cost) {}

    string getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    double getCost() const {
        return cost;
    }
};

class Menu {
private:
    string name;
    string ingredients;
    double price;
    int preparationTime;

public:
    Menu(const string& name, const string& ingredients, double price, int preparationTime)
        : name(name), ingredients(ingredients), price(price), preparationTime(preparationTime) {}

    string getName() const {
        return name;
    }

    string getIngredients() const {
        return ingredients;
    }

    double getPrice() const {
        return price;
    }

    int getPreparationTime() const {
        return preparationTime;
    }
};

class Order {
private:
    vector<Menu> items;
    bool paid;
    bool inProgress;
    bool ready;
    bool delivered;

public:
    Order() : paid(false), inProgress(false), ready(false), delivered(false) {}

    void addItem(const Menu& item) {
        items.push_back(item);
    }

    void markAsPaid() {
        paid = true;
        inProgress = true;
    }

    void markAsInProgress() {
        inProgress = true;
    }

    void markAsReady() {
        ready = true;
    }

    void markAsDelivered() {
        delivered = true;
    }

    bool isPaid() const {
        return paid;
    }

    bool isInProgress() const {
        return inProgress;
    }

    bool isReady() const {
        return ready;
    }

    bool isDelivered() const {
        return delivered;
    }

    vector<Menu> getItems() const {
        return items;
    }
};

class Guest {
private:
    string name;
    vector<Order> orders;

public:
    Guest(const string& name) : name(name) {}

    void createOrder(const vector<Menu>& menuItems) {
        Order order;
        for (const Menu& item : menuItems) {
            order.addItem(item);
        }
        order.markAsPaid();
        orders.push_back(order);
    }

    void trackOrderStatus() const {
        cout << "Статус заказов:\n";
        for (const Order& order : orders) {
            if (order.isPaid()) {
                cout << "Заказ оплачен и передан на кухню.\n";
            }
            if (order.isInProgress()) {
                cout << "Заказ в процессе приготовления.\n";
            }
            if (order.isReady()) {
                cout << "Заказ готов к выдаче.\n";
            }
            if (order.isDelivered()) {
                cout << "Заказ выполнен.\n";
            }
        }
    }
    vector<Order> getOrders() const {
        return orders;
    }
};

class Administrator {
private:
    map<string, Menu> menu;
    vector<Product> productList;
    map<string, string> employeeAccounts;
    vector<string> auditLog;

public:
    Administrator() {
        loadMenu();
        loadProductList();
        loadEmployeeAccounts();
    }

    bool isUsernameTaken(const string& username) const {
        auto it = employeeAccounts.find(username);
        return it != employeeAccounts.end();
    }

    void registerEmployee() {
        string username;
        string password;

        cout << "Введите имя пользователя: ";
        getline(cin, username);

        // Проверка, занято ли указанное имя пользователя
        if (isUsernameTaken(username)) {
            cout << "Ошибка! Имя пользователя уже занято.\n";
            return;
        }

        cout << "Введите пароль: ";
        getline(cin, password);

        // Регистрация нового сотрудника
        employeeAccounts[username] = password;

        // Сохранение обновленного списка учетных записей
        saveEmployeeAccounts(employeeAccounts);

        cout << "Регистрация успешно завершена.\n";
    }

    void saveEmployeeAccounts(const map<string, string>& employeeAccounts) {
        ofstream file("employee_records.txt");
        if (!file) {
            cout << "Ошибка сохранения учетных записей сотрудников.\n";
            return;
        }

        for (const auto& pair : employeeAccounts) {
            const string& username = pair.first;
            const string& password = pair.second;
            file << username << "\n" << password << "\n";
        }

        file.close();

        cout << "Учетные записи сотрудников успешно сохранены.\n";
    }

    map<string, Menu>& getMenu() {
        return menu;
    }

    map<string, string>& getEmployeeAccounts() {
        return employeeAccounts;
    }

    void loadMenu() {
        ifstream file("menu.txt");
        if (!file) {
            cout << "Ошибка загрузки menu.txt.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            string name = line;
            string ingredients;
            double price;
            int preparationTime;

            if (getline(file, line)) {
                ingredients = line;
            }
            if (getline(file, line)) {
                try {
                    price = stod(line);
                } catch (const std::invalid_argument& e) {
                    cout << "Ошибка преобразования цены: " << e.what() << " (" << line << ")" << endl;
                    continue;  // Продолжаем чтение следующей записи
                }
            }
            if (getline(file, line)) {
                try {
                    preparationTime = stoi(line);
                } catch (const std::invalid_argument& e) {
                    cout << "Ошибка преобразования времени приготовления: " << e.what() << " (" << line << ")" << endl;
                    continue;  // Продолжаем чтение следующей записи
                }
            }

            Menu menuItem(name, ingredients, price, preparationTime);
            menu.insert(make_pair(name, menuItem));
        }

        file.close();

        if (menu.empty()) {
            cout << "Ошибка: меню не загружено." << endl;
        } else {
            cout << "Меню успешно загружено." << endl;
        }
    }

    void loadProductList() {
        ifstream file("product_list.txt");
        if (!file) {
            cout << "Ошибка загрузки списка продуктов product_list.txt.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            string id = line;
            string name;
            double cost;

            if (getline(file, line)) {
                name = line;
            }
            if (getline(file, line)) {
                try {
                    cost = stod(line);
                } catch (const std::invalid_argument& e) {
                    cout << "Ошибка преобразования стоимости: " << e.what() << " (" << line << ")" << endl;
                    continue;  // Продолжаем чтение следующей записи
                }
            }

            Product product(id, name, cost);
            productList.push_back(product);
        }

        file.close();

        if (!productList.empty()) {
            cout << "Список продуктов успешно загружен." << endl;
        } else {
            cout << "Ошибка: список продуктов не загружен." << endl;
        }
    }

    void loadEmployeeAccounts() {
        ifstream file("employee_records.txt");
        if (!file) {
            cout << "Ошибка загрузки учетных записей сотрудников employee_records.txt.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            string username = line;
            string password;

            if (getline(file, line)) {
                password = line;
            }

            employeeAccounts[username] = password;
        }

        file.close();

        if (!employeeAccounts.empty()) {
            cout << "Учетные записи сотрудников успешно загружены." << endl;
        } else {
            cout << "Ошибка: учетные записи сотрудников не загружены." << endl;
        }
    }



    void editMenu() {
        string name;
        string ingredients;
        double price;
        int preparationTime;

        cout << "Введите наименование блюда: ";
        getline(cin, name);

        cout << "Введите состав блюда: ";
        getline(cin, ingredients);

        cout << "Введите стоимость блюда: ";
        cin >> price;

        cout << "Введите время приготовления блюда: ";
        cin >> preparationTime;

        Menu menuItem(name, ingredients, price, preparationTime);
        menu.insert(make_pair(name, menuItem));


        cout << "Меню успешно отредактировано.\n";
    }

    void editProductList() {
        string id;
        string name;
        double cost;

        cout << "Введите id продукта: ";
        getline(cin >> ws, id);

        cout << "Введите наименование продукта: ";
        getline(cin, name);

        cout << "Введите стоимость продукта: ";
        cin >> cost;

        Product product(id, name, cost);
        productList.push_back(product);

        cout << "Список продуктов успешно отредактирован.\n";
    }

    void editEmployeeAccounts() {
        string username;
        string password;

        cout << "Введите имя пользователя: ";
        getline(cin, username);

        cout << "Введите пароль: ";
        getline(cin, password);

        employeeAccounts[username] = password;

        cout << "Учетные записи сотрудников успешно отредактированы.\n";
    }

    void writeAuditLog(const string& action) {
        auditLog.push_back(action);
    }
};

class StockWorker {
private:
    vector<Product> products;
    double balance;

public:
    StockWorker() {
    }
    void createProductRequest() {
        cout << "Список доступных продуктов:\n";
        for (const Product& product : products) {
            cout << "ID: " << product.getId() << ", Название: " << product.getName() << ", Стоимость: " << product.getCost() << "\n";
        }

        string productId;
        int quantity;

        cout << "Введите ID продукта: ";
        getline(cin >> ws, productId);

        cout << "Введите количество: ";
        cin >> quantity;

        double totalCost = getProductCost(productId) * quantity;

        if (totalCost > getBalance()) {
            cout << "Ошибка! Баланс денежных средств ресторана недостаточен.\n";
            return;
        }

        decreaseBalance(totalCost);
        updateProductQuantity(productId, quantity);

        cout << "Заявка на продукты успешно создана.\n";
    }

    double getBalance() const {
        return balance;
    }

    double getProductCost(const string& productId) const {
        auto it = find_if(products.begin(), products.end(), [&](const Product& product) {
            return product.getId() == productId;
        });

        if (it != products.end()) {
            return it->getCost();
        }

        return 0.0;
    }

    void decreaseBalance(double amount) {
        balance -= amount;
    }

    void updateProductQuantity(const string& productId, int quantity) {
        auto it = find_if(products.begin(), products.end(), [&](const Product& product) {
            return product.getId() == productId;
        });

        if (it != products.end()) {
            // Уменьшаем количество продукта на складе
            // Например, it->setQuantity(it->getQuantity() - quantity);
        }
    }
};

class Accountant {
private:
    double balance;

public:
    void calculateBalance(const vector<Order>& orders) {
        double totalIncome = 0.0;

        for (const Order& order : orders) {
            if (order.isDelivered()) {
                for (const Menu& item : order.getItems()) {
                    totalIncome += item.getPrice();
                }
            }
        }

        balance = totalIncome;
    }

    double getBalance() const {
        return balance;
    }
};

// Функция для проверки доступа к функционалу администратора
bool checkAdminAccess(const map<string, string>& employeeAccounts) {
    string username;
    string password;

    cout << "Введите имя пользователя: ";
    getline(cin, username);

    cout << "Введите пароль: ";
    getline(cin, password);

    auto it = employeeAccounts.find(username);
    if (it != employeeAccounts.end() && it->second == password) {
        return true;
    }

    cout << "Ошибка! Неверное имя пользователя или пароль.\n";
    return false;
}

int main() {
    Guest guest("Иван");
    Administrator admin;
    StockWorker stockWorker;
    Accountant accountant;

    while (true) {
        std::cout << "          __\n"
                     " \\ ______/ V`-,\n"
                     "  }        /~~\n"
                     " /_)^ --,r'\n"
                     "|b      |b\n";
        cout << "1. Создать заказ\n";
        cout << "2. Отследить статус заказов\n";
        cout << "3. Редактировать меню (только для администратора)\n";
        cout << "4. Редактировать список продуктов (только для администратора)\n";
        cout << "5. Создать заявку на продукты (только для работника склада)\n";
        cout << "6. Рассчитать баланс (только для бухгалтера)\n";
        cout << "7. Рагистрация сотрудника (только для администратора)\n";
        cout << "0. Выйти\n";
        cout << "Выберите действие: ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                // Создание заказа
                vector<Menu> menuItems;

                cout << "Список доступных блюд:\n";
                for (const auto& pair : admin.getMenu()) {
                    const Menu& menuItem = pair.second;
                    cout << "Название: " << menuItem.getName() << ", Стоимость: " << menuItem.getPrice() << "\n";
                }

                while (true) {
                    string itemName;
                    cout << "Введите название блюда (0 - завершить выбор): ";
                    getline(cin, itemName);

                    if (itemName == "0") {
                        break;
                    }

                    auto it = admin.getMenu().find(itemName);
                    if (it != admin.getMenu().end()) {
                        menuItems.push_back(it->second);
                    } else {
                        cout << "Ошибка! Некорректное название блюда.\n";
                    }
                }

                guest.createOrder(menuItems);
                cout << "Заказ успешно создан.\n";
                break;
            }
            case 2: {
                // Отслеживание статуса заказов
                guest.trackOrderStatus();
                break;
            }
            case 3: {
                // Редактирование меню (только для администратора)
                if (checkAdminAccess(admin.getEmployeeAccounts())) {
                    admin.editMenu();
                }
                break;
            }
            case 4: {
                // Редактирование списка продуктов (только для администратора)
                if (checkAdminAccess(admin.getEmployeeAccounts())) {
                    admin.editProductList();
                }
                break;
            }
            case 5: {
                // Создание заявки на продукты (только для работника склада)
                stockWorker.createProductRequest();
                break;
            }
            case 6: {
                // Рассчет баланса (только для бухгалтера)
                accountant.calculateBalance(guest.getOrders());
                cout << "Баланс ресторана: " << accountant.getBalance() << "\n";
                break;
            }
            case 7: {
                // Регистрация сотрудника (только для администратора)
                if (checkAdminAccess(admin.getEmployeeAccounts())) {
                    admin.registerEmployee();
                }
                break;
            }
            case 0: {
                // Выход из программы
                cout << "До свидания!\n";
                return 0;
            }
            default:
                cout << "Ошибка! Некорректный выбор.\n";
                break;
        }
    }
}