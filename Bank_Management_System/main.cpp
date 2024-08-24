#include "BankSystem.h"

int main() {
    Bank bank;

    // Load accounts and transactions from files
    bank.loadAccountsFromFile("accounts.txt");
    bank.loadTransactionsFromFile("transactions.txt");

    // Displaying accounts and transactions
    cout << "Bank Accounts:" << endl;
    bank.displayAccounts();

    cout << "Bank Transactions:" << endl;
    bank.displayTransactions();

    return 0;
}
