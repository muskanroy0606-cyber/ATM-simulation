   
#include <iostream>  
#include <fstream>       
#include <string>
#include <iomanip>  
#include <ctime>
using namespace std;

// 🎨 Color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define WHITE   "\033[37m"
#define BOLDWHITE "\033[1m\033[37m"

// Account structure
struct Account {
    string accNumber;
    string pin;
    double balance;
};

// Load account data
bool loadAccount(Account &acc) {
    ifstream file("account.txt");
    if (!file.is_open()) return false;

    file >> acc.accNumber >> acc.pin >> acc.balance;
    file.close();

    // Check if file contains valid data
    if (acc.accNumber.empty() || acc.pin.empty())
        return false;

    return true;
}

// Save account data
void saveAccount(const Account &acc) {
    ofstream file("account.txt");
    if (file.is_open()) {
        file << acc.accNumber << " " << acc.pin << " " << acc.balance;
        file.close();
    }
}

// Clear screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Simple delay
void delay(int ms) {
    clock_t end_time = clock() + ms * CLOCKS_PER_SEC / 1000;
    while (clock() < end_time);
}

// Loading effect
void loading(string message) {
    cout << YELLOW << message;
    for (int i = 0; i < 3; i++) {
        cout << ".";
        cout.flush();
        delay(300);
    }
    cout << RESET << endl;
    delay(300);
}

// Header
void showHeader() {
    cout << CYAN << "=========================================\n";
    cout << BLUE << "           MINI ATM SIMULATION           \n";
    cout << CYAN << "=========================================\n" << RESET;
    cout << endl;
}

int main() {
    Account acc;

    // Load or create account file
    if (!loadAccount(acc)) {
        acc = {"123456", "0000", 1000.00};
        saveAccount(acc);
    }

    string inputAcc, inputPin;
    clearScreen();
    showHeader();

    cout << WHITE << "Enter Account Number: " << RESET;
    cin >> inputAcc;
    cout << WHITE << "Enter PIN: " << RESET;
    cin >> inputPin;

    // Login check
    if (inputAcc != acc.accNumber || inputPin != acc.pin) {
        cout << RED << "\nInvalid Account Number or PIN!\n" << RESET;
        cout << YELLOW << "\n(If this is your first run, please delete old 'account.txt' file.)\n" << RESET;
        return 0;
    }

    int choice;
    double amount;

    do {
        clearScreen();
        showHeader();
        cout << BOLDWHITE << "Welcome, Account: " << acc.accNumber << "\n\n" << RESET;

        cout << YELLOW << "[1]" << WHITE << " Check Balance\n";
        cout << YELLOW << "[2]" << WHITE << " Deposit Money\n";
        cout << YELLOW << "[3]" << WHITE << " Withdraw Money\n";
        cout << YELLOW << "[4]" << WHITE << " Change PIN\n";
        cout << YELLOW << "[5]" << WHITE << " Exit\n";
        cout << "\nEnter your choice: " << RESET;
        cin >> choice;

        switch (choice) {
            case 1:
                loading("Fetching balance");
                cout << GREEN << "\nCurrent Balance: ₹" << fixed << setprecision(2) << acc.balance << RESET << endl;
                break;

            case 2:
                cout << CYAN << "\nEnter amount to deposit: ₹" << RESET;
                cin >> amount;
                if (amount <= 0) {
                    cout << RED << "Invalid amount!" << RESET << endl;
                    break;
                }
                acc.balance += amount;
                loading("Processing deposit");
                cout << GREEN << "Deposit successful! New balance: ₹" << acc.balance << RESET << endl;
                break;

            case 3:
                cout << CYAN << "\nEnter amount to withdraw: ₹" << RESET;
                cin >> amount;
                loading("Processing withdrawal");
                if (amount <= 0) {
                    cout << RED << "Invalid amount!" << RESET << endl;
                } else if (amount > acc.balance) {
                    cout << RED << "Insufficient Balance!" << RESET << endl;
                } else {
                    acc.balance -= amount;
                    cout << GREEN << "Withdraw successful! Remaining balance: ₹" << acc.balance << RESET << endl;
                }
                break;

            case 4: {
                string oldPin, newPin;
                cout << YELLOW << "\nEnter Old PIN: " << RESET;
                cin >> oldPin;
                if (oldPin == acc.pin) {
                    cout << YELLOW << "Enter New PIN: " << RESET;
                    cin >> newPin;
                    acc.pin = newPin;
                    loading("Updating PIN");
                    cout << GREEN << "PIN changed successfully!" << RESET << endl;
                } else {
                    cout << RED << "Incorrect Old PIN!" << RESET << endl;
                }
                break;
            }

            case 5:
                loading("Exiting");
                cout << MAGENTA << "\nThank you for using Mini ATM!" << RESET << endl;
                break;

            default:
                cout << RED << "\nInvalid Choice!" << RESET << endl;
        }

        saveAccount(acc);
        if (choice != 5) {
            cout << YELLOW << "\nPress Enter to continue..." << RESET;
            cin.ignore();
            cin.get();
        }

    } while (choice != 5);

    return 0;
}
