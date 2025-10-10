// Brandon Cotesta 10/08/2025

#include "Account.h"
#include "Transaction.h" 
#include <iostream>
#include <string>
#include <chrono>
using namespace std;

Account::Account(string accountNumber, AccountType accType)
{
    accountNumber_ = std::move(accountNumber);
    accountType_ = accType;
    balance_ = 0.0;
}

// Getters
const string& Account::accountNumber() const { return accountNumber_; }
AccountType Account::accountType() const { return accountType_; }
list<Transaction>& Account::transactionHistory() { return transactionHistory_; }
const list<Transaction>& Account::transactionHistory() const { return transactionHistory_; }

// Deposit into the account
void Account::deposit(double amount)
{
    if (amount <= 0) {
        cerr << "Deposit amount must be positive." << endl;
        return;
    }
    balance_ += amount;

    // Use a more robust ID generation
    static int nextId = 1;
    int newId = nextId++;
    DateTime now = std::chrono::system_clock::now();
    Transaction depositTransaction(newId, now, amount, TransactionType::DEPOSIT, "Deposit - " + to_string(newId));
    transactionHistory_.push_back(depositTransaction);
    depositTransaction.process();

    cout << "Deposited: $" << amount << ", New Balance: $" << balance_ << endl;
}

// Withdraw from the account
void Account::withdraw(double amount)
{
    if (amount <= 0) {
        cerr << "Withdrawal amount must be positive." << endl;
        return;
    }
    if (amount > balance_) {
        cerr << "Insufficient funds for withdrawal." << endl;
        return;
    }
    balance_ -= amount;

    // Use a more robust ID generation
    static int nextId = 1;
    int newId = nextId++;
    DateTime now = std::chrono::system_clock::now();
    Transaction withdrawalTransaction(newId, now, amount, TransactionType::WITHDRAWAL, "Withdrawal - " + to_string(newId));
    transactionHistory_.push_back(withdrawalTransaction);
    withdrawalTransaction.process();

    cout << "Withdrew: $" << amount << ", New Balance: $" << balance_ << endl;
}

double Account::getBalance() const { return balance_; }
