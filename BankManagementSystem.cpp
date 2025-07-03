#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
using namespace std;

// Get current date-time
string getCurrentTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", ltm);
    return string(buffer);
}

class BankAccount {
    string accountNumber, name, password;
    double balance = 0.0;

public:
    void createAccount() {
        cout << "Enter Account Number: ";
        cin >> accountNumber;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Create Password: ";
        cin >> password;
        cout << "Enter Initial Balance: ";
        cin >> balance;

        ofstream out("accounts.txt", ios::app);
        out << accountNumber << " " << name << " " << password << " " << balance << endl;
        out.close();

        ofstream hist(accountNumber + "_history.txt");
        hist << "Date/Time           | Action    | Amount   | Balance\n";
        hist << "-----------------------------------------------------\n";
        hist << getCurrentTime() << " | Created   | $" << balance << " | $" << balance << endl;
        hist.close();

        cout << "? Account created successfully!\n";
    }

    bool authenticate() {
        string acc, pass;
        cout << "Enter Account Number: ";
        cin >> acc;
        cout << "Enter Password: ";
        cin >> pass;

        ifstream in("accounts.txt");
        string a, n, p;
        double b;
        while (in >> a >> n >> p >> b) {
            if (a == acc && p == pass) {
                accountNumber = a;
                name = n;
                password = p;
                balance = b;
                return true;
            }
        }
        cout << "? Invalid account number or password.\n";
        return false;
    }

    void deposit() {
        if (!authenticate()) return;
        double amt;
        cout << "Enter amount to deposit: ";
        cin >> amt;
        balance += amt;
        updateAccount();
        logTransaction("Deposit", amt);
        cout << "? Deposit successful. New balance: $" << balance << endl;
    }

    void withdraw() {
        if (!authenticate()) return;
        double amt;
        cout << "Enter amount to withdraw: ";
        cin >> amt;
        if (amt > balance) {
            cout << "? Insufficient funds.\n";
            return;
        }
        balance -= amt;
        updateAccount();
        logTransaction("Withdraw", amt);
        cout << "? Withdrawal successful. New balance: $" << balance << endl;
    }

    void checkBalance() {
        if (!authenticate()) return;
        cout << "?? Current Balance: $" << balance << endl;
    }

    void viewHistory() {
        if (!authenticate()) return;
        ifstream hist(accountNumber + "_history.txt");
        string line;
        while (getline(hist, line)) {
            cout << line << endl;
        }
    }

private:
    void updateAccount() {
        ifstream in("accounts.txt");
        ofstream temp("temp.txt");
        string a, n, p;
        double b;
        while (in >> a >> n >> p >> b) {
            if (a == accountNumber)
                temp << a << " " << name << " " << password << " " << balance << endl;
            else
                temp << a << " " << n << " " << p << " " << b << endl;
        }
        in.close();
        temp.close();
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
    }

    void logTransaction(const string& type, double amt) {
        ofstream hist(accountNumber + "_history.txt", ios::app);
        hist << getCurrentTime() << " | " << setw(9) << type << " | $" << setw(7)
             << fixed << setprecision(2) << amt << " | $" << balance << endl;
    }
};

int main() {
    BankAccount b;
    int ch;

    while (true) {
        cout << "\n=== Bank Account Management System ===\n";
        cout << "1. Create Account\n2. Deposit\n3. Withdraw\n4. Check Balance\n";
        cout << "5. View Transaction History\n6. Exit\n";
        cout << "Enter your choice: ";
        cin >> ch;

        switch (ch) {
            case 1: b.createAccount(); break;
            case 2: b.deposit(); break;
            case 3: b.withdraw(); break;
            case 4: b.checkBalance(); break;
            case 5: b.viewHistory(); break;
            case 6: cout << "?? Thank you! Exiting...\n"; return 0;
            default: cout << "? Invalid choice.\n";
        }
    }
}

