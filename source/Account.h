#pragma once
#include "Types.h"
#include <Transaction.cpp>
#include <string>
#include <list>
class Account
{
public:
	Account(string accountNumber, AccountType accType);
	string accountNumber() const;
	AccountType accountType() const;
	list<Transaction> transactionHistory() const;
	// functions
	void deposit(double amount);
	void withdraw(double amount);
	double getBalance();
private:
	string accountNumber_;
	AccountType accountType_;
	list<Transaction> transactionHistory_;
	double balance_;
};

