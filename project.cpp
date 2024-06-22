#include <iostream>
#include <string>
#include <iomanip>
#include <exception>
#include <fstream>
#include <sstream>
#include <conio.h>

using namespace std;

const int MAX_USERS = 10;
string names[MAX_USERS];
string addresses[MAX_USERS];
string emails[MAX_USERS];
string phoneNumbers[MAX_USERS];
string usernames[MAX_USERS];
string passwords[MAX_USERS];
int userCount = 0;

class product {
public:
    string name;
    double price;
    bool instock;

    // Default constructor
    product() : name(""), price(0.0), instock(false) {}

    // Parameterized constructor
    product(const string& name, double price, bool instock) : name(name), price(price), instock(instock) {}
};

class cart : public product {
public:
    product items[10];
    int item_count = 0;

    // Default constructor
    cart() {}

    void additems(const product& pr) {
        if (item_count < 10) {
            items[item_count++] = pr;
            cout << "Added " << pr.name << " to your cart." << endl;
        }
        else {
            cout << "Cart is full" << endl;
        }
    }

    void viewCart() {
        if (item_count == 0) {
            cout << "Your cart is empty." << endl;
        }
        else {
            cout << "Your cart items:" << endl;
            for (int i = 0; i < item_count; i++) {
                cout << i + 1 << ". " << items[i].name << " - $" << fixed << setprecision(2) << items[i].price << endl;
            }
        }
    }

    double getTotalPrice() {
        double total = 0.0;
        for (int i = 0; i < item_count; i++) {
            total += items[i].price;
        }
        return total;
    }

    int getItemCount() {
        return item_count;
    }

    void clearCart() {
        item_count = 0;
    }
};

class confirmation {
public:
    void confirmOrder(cart& c) {
        if (c.getItemCount() == 0) {
            cout << "Your cart is empty. Add items to cart before confirming the order." << endl;
            return;
        }

        string deliveryAddress;
        cout << "Enter your complete address for delivery: ";
        cin.ignore();
        getline(cin, deliveryAddress);

        double totalPrice = c.getTotalPrice();
        const double deliveryFee = 30.0;
        totalPrice += deliveryFee;

        char paymentMethod;
        cout << "Choose a payment method:" << endl;
        cout << "Enter 'D' for cash via debit card (10% discount)" << endl;
        cout << "Enter 'C' for cash on delivery" << endl;
        cin >> paymentMethod;

        if (paymentMethod == 'D' || paymentMethod == 'd') {
            totalPrice *= 0.90; // 10% discount
            cout << "10% discount applied." << endl;
        }

        cout << "\nOrder Summary:" << endl;
        cout << "-------------------------------------" << endl;
        cout << setw(20) << left << "Item"
            << setw(10) << right << "Price" << endl;
        cout << "-------------------------------------" << endl;
        for (int i = 0; i < c.getItemCount(); i++) {
            cout << setw(20) << left << c.items[i].name
                << setw(10) << right << fixed << setprecision(2) << c.items[i].price << endl;
        }
        cout << "-------------------------------------" << endl;
        cout << setw(20) << left << "Delivery Fee"
            << setw(10) << right << fixed << setprecision(2) << deliveryFee << endl;
        cout << setw(20) << left << "Total Price"
            << setw(10) << right << fixed << setprecision(2) << totalPrice << endl;
        cout << "-------------------------------------" << endl;
        cout << "Order confirmed. Thank you for your purchase!" << endl;
        cout << "Your order will be delivered to: " << deliveryAddress << endl;

        saveOrderDetails(c, deliveryAddress, totalPrice);
        c.clearCart();
    }

    void saveOrderDetails(cart& c, const string& address, double totalPrice) {
        try {
            ofstream outFile("order_details.txt", ios::app);
            if (!outFile) {
                throw runtime_error("Error opening file for writing.");
            }
            outFile << "Order Details:" << endl;
            for (int i = 0; i < c.getItemCount(); i++) {
                outFile << c.items[i].name << " - $" << fixed << setprecision(2) << c.items[i].price << endl;
            }
            outFile << "Delivery Address: " << address << endl;
            outFile << "Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
            outFile << "-------------------------------------" << endl;
            outFile.close();
        }
        catch (const exception& e) {
            cerr << "An error occurred: " << e.what() << endl;
        }
    }
};

void saveUsersToFile() {
    try {
        ofstream outFile("users.txt");
        if (!outFile) {
            throw runtime_error("Error opening file for writing.");
        }
        for (int i = 0; i < userCount; i++) {
            outFile << names[i] << "," << addresses[i] << "," << emails[i] << "," << phoneNumbers[i] << "," << usernames[i] << "," << passwords[i] << endl;
        }
        outFile.close();
    }
    catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
    }
}

void loadUsersFromFile() {
    try {
        ifstream inFile("users.txt");
        if (!inFile) {
            throw runtime_error("Error opening file for reading.");
        }
        string line;
        while (getline(inFile, line) && userCount < MAX_USERS) {
            stringstream ss(line); // use stringstream to parse the line
            getline(ss, names[userCount], ',');
            getline(ss, addresses[userCount], ',');
            getline(ss, emails[userCount], ',');
            getline(ss, phoneNumbers[userCount], ',');
            getline(ss, usernames[userCount], ',');
            getline(ss, passwords[userCount], ',');
            userCount++;
        }
        inFile.close();
    }
    catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
    }
}

void signup() {
    if (userCount >= MAX_USERS) {
        cout << "User limit reached. Cannot sign up more users." << endl;
        return;
    }

    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, names[userCount]);
    cout << "Enter your address: ";
    getline(cin, addresses[userCount]);
    cout << "Enter your email: ";
    getline(cin, emails[userCount]);
    cout << "Enter your phone number: ";
    getline(cin, phoneNumbers[userCount]);
    cout << "Enter a username: ";
    getline(cin, usernames[userCount]);
    cout << "Enter a password: ";
    getline(cin, passwords[userCount]);

    userCount++;
    cout << "Sign up successful!" << endl;
    saveUsersToFile();
}

void deleteUserAccount() {
    string username, password;
    cout << "Enter your username: ";
    cin.ignore();
    getline(cin, username);
    cout << "Enter your password: ";
    getline(cin, password);

    for (int i = 0; i < userCount; i++) {
        if (usernames[i] == username && passwords[i] == password) {
            for (int j = i; j < userCount - 1; j++) {
                names[j] = names[j + 1];
                addresses[j] = addresses[j + 1];
                emails[j] = emails[j + 1];
                phoneNumbers[j] = phoneNumbers[j + 1];
                usernames[j] = usernames[j + 1];
                passwords[j] = passwords[j + 1];
            }
            userCount--;
            cout << "Account deleted successfully." << endl;
            saveUsersToFile();
            return;
        }
    }
    cout << "Failed to delete account. Incorrect username or password." << endl;
}

bool login() {
    string u_name, pass;
    char ch;
    cout << "Enter username and password" << endl;
    cout << "Enter username: ";
    cin.ignore();
    getline(cin, u_name);
    cout << "Enter password: ";
    ch = _getch();
    while (ch != '\r') {
        pass.push_back(ch);
        cout << '*';
        ch = _getch();
    }
    cout << endl; // add a new line after password entry
    for (int i = 0; i < userCount; i++) {
        if (usernames[i] == u_name && passwords[i] == pass) {
            cout << "Login successful! Welcome, " << names[i] << "!" << endl;
            return true;
        }
    }
    cout << "Login failed. Incorrect username or password." << endl;
    return false;
}

int main() {
    loadUsersFromFile();
    char choice1, choice2;
    cart c;
    confirmation conf;
    const int numProd = 5;
    product prod[numProd] = {
        {"Iphone", 10.99, true},
        {"TCL TV", 19.99, false},
        {"Headphone", 8.50, true},
        {"Watch", 15.75, true},
        {"Glasses", 5.25, false}
    };
    cout << "----- Shopping System -----" << endl;
    do {
        cout << endl;
        cout << "Select:" << endl;
        cout << "Press 'L' to log in" << endl;
        cout << "Press 'S' to sign up" << endl;
        cout << "Press 'D' to delete account" << endl;
        cout << "Press 'E' to exit" << endl;
        cout << "Your choice: ";
        cin >> choice1;
        cout << endl;

        if (choice1 == 's' || choice1 == 'S') {
            signup();
        }
        else if (choice1 == 'l' || choice1 == 'L') {
            if (login()) {
                do {
                    cout << endl;
                    cout << "Select your choice: " << endl;
                    cout << "Enter 'V' to view products" << endl;
                    cout << "Enter 'B' to buy product" << endl;
                    cout << "Enter 'C' to view cart" << endl;
                    cout << "Enter 'F' to confirm purchase" << endl;
                    cout << "Enter 'E' to exit" << endl;
                    cout << "Your choice: ";
                    cin >> choice2;
                    if (choice2 == 'V' || choice2 == 'v') {
                        cout << "Available Products:" << endl;
                        for (int i = 0; i < numProd; i++) {
                            cout << i + 1 << ". " << prod[i].name << " - $" << fixed << setprecision(2) << prod[i].price;
                            if (prod[i].instock) {
                                cout << " (In stock)" << endl;
                            }
                            else {
                                cout << " (Out of stock)" << endl;
                            }
                        }
                    }
                    else if (choice2 == 'B' || choice2 == 'b') {
                        int choice3;
                        cout << "Enter the index of the product you want to buy: ";
                        cin >> choice3;
                        if (choice3 >= 1 && choice3 <= numProd) {
                            if (prod[choice3 - 1].instock) {
                                c.additems(prod[choice3 - 1]);
                            }
                            else {
                                cout << "Sorry, " << prod[choice3 - 1].name << " is out of stock" << endl;
                            }
                        }
                        else {
                            cout << "Invalid product index" << endl;
                        }
                    }
                    else if (choice2 == 'C' || choice2 == 'c') {
                        c.viewCart();
                    }
                    else if (choice2 == 'F' || choice2 == 'f') {
                        conf.confirmOrder(c);
                    }
                } while (choice2 != 'e' && choice2 != 'E');
            }
        }
        else if (choice1 == 'd' || choice1 == 'D') {
            deleteUserAccount();
        }
        else if (choice1 != 'e' && choice1 != 'E') {
            cout << "Invalid choice" << endl;
        }
    } while (choice1 != 'e' && choice1 != 'E');
    return 0;
}
