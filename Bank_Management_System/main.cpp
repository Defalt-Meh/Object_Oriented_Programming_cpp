#include "BankSystem.h"

int main() {
    Bank bank;

    // Adding accounts
    bank.addAccount("Alice", 500.00);
    bank.addAccount("Bob", 300.00);

    // Displaying accounts
    cout << "Bank Accounts:" << endl;
    bank.displayAccounts();

    // Making transactions
    bank.deposit(1, 200.00);
    bank.withdraw(2, 50.00);

    // Displaying transactions
    cout << "Bank Transactions:" << endl;
    bank.displayTransactions();

    // Removing an account
    bank.removeAccount(2);

    // Displaying accounts after removal
    cout << "Bank Accounts After Removal:" << endl;
    bank.displayAccounts();

    return 0;
}
