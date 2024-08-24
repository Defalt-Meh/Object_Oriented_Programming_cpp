#include "BankSystem.h"

// Account class definitions
Account::Account(int num, const string& name, double bal) 
    : accountNumber(num), owner(name), balance(bal) {}

int Account::getAccountNumber() const { return accountNumber; }
string Account::getOwner() const { return owner; }
double Account::getBalance() const { return balance; }

void Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
    }
}

bool Account::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        return true;
    }
    return false;
}

void Account::displayAccount() const {
    cout << "Account Number: " << accountNumber << endl;
    cout << "Owner: " << owner << endl;
    cout << "Balance: $" << balance << endl;
}

// Transaction class definitions
Transaction::Transaction(int id, int accNum, const string& transType, double amt) 
    : transactionID(id), accountNumber(accNum), type(transType), amount(amt) {}

int Transaction::getTransactionID() const { return transactionID; }
int Transaction::getAccountNumber() const { return accountNumber; }
string Transaction::getType() const { return type; }
double Transaction::getAmount() const { return amount; }

void Transaction::displayTransaction() const {
    cout << "Transaction ID: " << transactionID << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Type: " << type << endl;
    cout << "Amount: $" << amount << endl;
}

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
