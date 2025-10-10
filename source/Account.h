#pragma once
#include "Types.h"
#include "Transaction.h"
#include <string>
#include <list>

class Account
{
public:
    Account(std::string accountNumber, AccountType accType);

    const std::string& accountNumber() const;
    AccountType accountType() const;

    // Expose references if external code must inspect/append history
    std::list<Transaction>& transactionHistory();
    const std::list<Transaction>& transactionHistory() const;

    void deposit(double amount);
    void withdraw(double amount);
    double getBalance() const;

private:
    std::string accountNumber_;
    AccountType accountType_;
    std::list<Transaction> transactionHistory_;
    double balance_{0.0};
};

