#ifndef BANKSYSTEM_H
#define BANKSYSTEM_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Account {
private:
    int accountNumber;
    string owner;
    double balance;

public:
    Account(int num, const string& name, double bal);
    
    int getAccountNumber() const;
    string getOwner() const;
    double getBalance() const;
    
    void deposit(double amount);
    bool withdraw(double amount);
    void displayAccount() const;
};

class Transaction {
private:
    int transactionID;
    int accountNumber;
    string type;
    double amount;

public:
    Transaction(int id, int accNum, const string& transType, double amt);
    
    int getTransactionID() const;
    int getAccountNumber() const;
    string getType() const;
    double getAmount() const;
    void displayTransaction() const;
};

class Bank {
private:
    vector<Account> accounts;
    vector<Transaction> transactions;
    int nextAccountNumber;
    int nextTransactionID;

public:
    Bank();
    
    void addAccount(const string& owner, double initialBalance);
    bool removeAccount(int accountNumber);
    
    bool deposit(int accountNumber, double amount);
    bool withdraw(int accountNumber, double amount);
    
    void displayAccounts() const;
    void displayTransactions() const;
    void recordTransaction(int accountNumber, const string& type, double amount);
};

#endif // BANKSYSTEM_H
