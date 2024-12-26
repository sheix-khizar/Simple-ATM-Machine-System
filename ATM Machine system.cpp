#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <unistd.h> // For sleep() function
using namespace std;

// Utility function for password encryption (simple XOR method)
string encryptDecrypt(string data, char key = 'K') {
    for (size_t i = 0; i < data.length(); i++) {
        data[i] ^= key;
    }
    return data;
}

// Base class Person
class Person {
protected:
    string name;
    int age;
    string gender;

public:
    // Default constructor
    Person() : name(""), age(0), gender("") {}

    // Parameterized constructor
    Person(string n, int a, string g) : name(n), age(a), gender(g) {}

    // Display method
    virtual void display() {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Gender: " << gender << endl;
    }

    // Write data to file
    void writeToFile() {
        ofstream outFile("person.txt");
        outFile << name << endl << age << endl << gender << endl;
        outFile.close();
    }

    // Read data from file
    void readFromFile() {
        ifstream inFile("person.txt");
        inFile >> name >> age >> gender;
        inFile.close();
    }
};

// Derived class User from Person
class User : public Person {
protected:
    string username;
    string password;
    string pin; // Added PIN for ATM login

public:
    // Default constructor
    User() : Person(), username(""), password(""), pin("") {}

    // Parameterized constructor
    User(string n, int a, string g, string uname, string pwd, string p) 
        : Person(n, a, g), username(uname), password(pwd), pin(p) {}

    // Login method using username, password and PIN
    bool login(string uname, string pwd, string p) {
        return (username == uname && encryptDecrypt(password) == pwd && pin == p);
    }

    // Override display method
    void display() {
        Person::display();
        cout << "Username: " << username << endl;
    }

    // Set password and PIN
    void setPassword(string pwd) {
        password = encryptDecrypt(pwd); // Encrypting password
    }

    void setPIN(string p) {
        pin = p;
    }
};

// Derived class BankAccount from User
class BankAccount : public User {
private:
    int accountNumber;
    string accountType;
    float balance;
    time_t lastLoginTime; // Track the last login time

public:
    // Default constructor
    BankAccount() : User(), accountNumber(0), accountType(""), balance(0.0) {}

    // Parameterized constructor
    BankAccount(string n, int a, string g, string uname, string pwd, string p, 
                int accNum, string accType, float bal) : User(n, a, g, uname, pwd, p),
                                                        accountNumber(accNum), 
                                                        accountType(accType), 
                                                        balance(bal) {}

    // Deposit method
    void deposit(float amount) {
        balance += amount;
        cout << "Deposited Rs/- " << amount << " in your account." << endl;
        cout << "New Balance: Rs/- " << balance << endl;
    }

    // Withdraw method
    void withdraw(float amount) {
        if (amount > balance) {
            cout << "Insufficient balance!" << endl;
        } else {
            balance -= amount;
            cout << "Withdrew Rs/- " << amount << " from your account." << endl;
            cout << "New Balance: Rs/- " << balance << endl;
        }
    }

    // Display account details
    void display() {
        User::display();
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Balance: Rs/- " << balance << endl;
    }

    // Display mini-statement (recent transactions)
    void miniStatement() {
        // Add logic to show last few transactions (for now, a placeholder)
        cout << "Mini Statement: Last 5 transactions displayed here." << endl;
    }

    // Logout session (reset login time)
    void logout() {
        lastLoginTime = 0;
        cout << "Logging out..." << endl;
    }

    // Timeout (if no action for 5 minutes, log out automatically)
    void checkTimeout() {
        time_t currentTime;
        time(&currentTime);
        if (difftime(currentTime, lastLoginTime) > 300) {  // 5 minutes timeout
            cout << "Session timed out due to inactivity. Logging out..." << endl;
            logout();
        }
    }

    // Save account data to file
    void writeToFile() {
        ofstream outFile("account.txt");
        outFile << name << endl << age << endl << gender << endl;
        outFile << username << endl << password << endl << pin << endl;
        outFile << accountNumber << endl << accountType << endl << balance << endl;
        outFile.close();
    }

    // Read account data from file
    void readFromFile() {
        ifstream inFile("account.txt");
        inFile >> name >> age >> gender;
        inFile >> username >> password >> pin;
        inFile >> accountNumber >> accountType >> balance;
        inFile.close();
    }
};

// Function to handle account actions like deposit, withdraw, etc.
void handleAccountActions(BankAccount& account) {
    int action;
    while (true) {
        cout << "\n-------- Account Actions --------" << endl;
        cout << "1. Deposit" << endl;
        cout << "2. Withdraw" << endl;
        cout << "3. Display Account Details" << endl;
        cout << "4. Mini Statement" << endl;
        cout << "5. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> action;

        if (action == 1) {
            float amount;
            cout << "Enter deposit amount: Rs/- ";
            cin >> amount;
            account.deposit(amount);
        } else if (action == 2) {
            float amount;
            cout << "Enter withdrawal amount: Rs/- ";
            cin >> amount;
            account.withdraw(amount);
        } else if (action == 3) {
            account.display();
        } else if (action == 4) {
            account.miniStatement();
        } else if (action == 5) {
            account.logout();
            break; // Exit to main menu
        } else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void createNewAccount(BankAccount& account) {
    string name, username, password, accountType, gender, pin;
    int age, accountNumber;
    float balance;
    
    // Get user input
    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your age: ";
    cin >> age;
    
    // Select gender from options
    cout << "Select gender: 1. Male  2. Female" << endl;
    int genderChoice;
    cin >> genderChoice;
    if (genderChoice == 1) {
        gender = "Male";
    } else if (genderChoice == 2) {
        gender = "Female";
    } else {
        cout << "Invalid choice! Defaulting to Male." << endl;
        gender = "Male";
    }

    // Select account type from options
    cout << "Select account type: 1. Current  2. Saving" << endl;
    int accountTypeChoice;
    cin >> accountTypeChoice;
    if (accountTypeChoice == 1) {
        accountType = "Current";
    } else if (accountTypeChoice == 2) {
        accountType = "Saving";
    } else {
        cout << "Invalid choice! Defaulting to Saving." << endl;
        accountType = "Saving";
    }

    // Get username, password, pin, account number, and balance
    cout << "Set a username: ";
    cin >> username;
    cout << "Set a password: ";
    cin >> password;
    cout << "Set a PIN: ";
    cin >> pin;
    cout << "Enter your account number: ";
    cin >> accountNumber;

    // Give the user options for account balance
    cout << "Choose an account balance (in multiples of 5000):" << endl;
    cout << "1. 5000" << endl;
    cout << "2. 10000" << endl;
    cout << "3. 15000" << endl;
    cout << "4. 20000" << endl;
    cout << "5. 25000" << endl;

    int balanceChoice;
    cin >> balanceChoice;

    // Set the balance based on the user's choice
    switch (balanceChoice) {
        case 1:
            balance = 5000;
            break;
        case 2:
            balance = 10000;
            break;
        case 3:
            balance = 15000;
            break;
        case 4:
            balance = 20000;
            break;
        case 5:
            balance = 25000;
            break;
        default:
            cout << "Invalid choice! Defaulting to 5000." << endl;
            balance = 5000; // Default balance if the user enters an invalid choice
            break;
    }

    cout << "Account created with balance: " << balance << endl;

    // Create the BankAccount object
    account = BankAccount(name, age, gender, username, password, pin, accountNumber, accountType, balance);

    cout << "Account created successfully!" << endl;
}


int main() {
    BankAccount account;
    int choice;

    cout << "<-------------- Welcome to ATM Machine ------------->" << endl;

    while (true) {
        cout << "\n-------- Main Menu --------" << endl;
        cout << "1. Create New Account" << endl;
        cout << "2. Login to Existing Account" << endl;
        cout << "3. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            createNewAccount(account);
        } else if (choice == 2) {
            string username, password, pin;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            cout << "Enter PIN: ";
            cin >> pin;

            if (account.login(username, password, pin)) {
                handleAccountActions(account);
            } else {
                cout << "Invalid Login ID or PIN! Try again." << endl;
            }
        } else if (choice == 3) {
            cout << "Thank You for using our ATM services!" << endl;
            break; // Exit the program
        } else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}

