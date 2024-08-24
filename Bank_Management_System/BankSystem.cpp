#include "BankSystem.h"
#include <fstream>
#include <sstream>

// Account class definitions (unchanged)

// Transaction class definitions (unchanged)

// Bank class definitions
Bank::Bank() : nextAccountNumber(1), nextTransactionID(1) {}

void Bank::addAccount(const string& owner, double initialBalance) {
    accounts.push_back(Account(nextAccountNumber++, owner, initialBalance));
}

bool Bank::removeAccount(int accountNumber) {
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->getAccountNumber() == accountNumber) {
            accounts.erase(it);
            return true;
        }
    }
    return false;
}

bool Bank::deposit(int accountNumber, double amount) {
    for (auto& acc : accounts) {
        if (acc.getAccountNumber() == accountNumber) {
            acc.deposit(amount);
            recordTransaction(accountNumber, "Deposit", amount);
            return true;
        }
    }
    return false;
}

bool Bank::withdraw(int accountNumber, double amount) {
    for (auto& acc : accounts) {
        if (acc.getAccountNumber() == accountNumber) {
            if (acc.withdraw(amount)) {
                recordTransaction(accountNumber, "Withdrawal", amount);
                return true;
            }
        }
    }
    return false;
}

void Bank::displayAccounts() const {
    for (const auto& acc : accounts) {
        acc.displayAccount();
        cout << "--------------------" << endl;
    }
}

void Bank::displayTransactions() const {
    for (const auto& trans : transactions) {
        trans.displayTransaction();
        cout << "--------------------" << endl;
    }
}

void Bank::recordTransaction(int accountNumber, const string& type, double amount) {
    transactions.push_back(Transaction(nextTransactionID++, accountNumber, type, amount));
}

void Bank::loadAccountsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string owner;
        double balance;
        getline(ss, owner, ',');
        ss >> balance;
        addAccount(owner, balance);
    }
    file.close();
}

void Bank::loadTransactionsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        int accountNumber;
        string type;
        double amount;
        ss >> accountNumber;
        ss.ignore(1, ',');
        getline(ss, type, ',');
        ss >> amount;
        
        if (type == "Deposit") {
            deposit(accountNumber, amount);
        } else if (type == "Withdrawal") {
            withdraw(accountNumber, amount);
        }
    }
    file.close();
}
