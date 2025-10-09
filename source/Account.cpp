// Brandon Cotesta 10/08/2025

#include "Account.h"
#include <iostream>
using namespace std;

Account::Account(string accountNumber, AccountType accType)
{
	this->accountNumber_ = accountNumber;
	this->accountType_ = accType;
	this->balance_ = 0.0; // Initialize balance to zero
}

// Getters
string Account::accountNumber() const { return accountNumber_; }
AccountType Account::accountType() const { return accountType_; }
list<Transaction> Account::transactionHistory() const { return transactionHistory_; }

// Depoisit into the account
// Creates a deposit transaction and updates the balance
void Account::deposit(double amount)
{
	if (amount <= 0)
	{
		cerr << "Deposit amount must be positive." << endl;
		return;
	}
	balance_ += amount;
	// Create a transaction record
	int newId = transactionHistory_.size() + 1; // Simple ID generation
	DateTime now; // Assume DateTime has a default constructor for current time
	Transaction depositTransaction(newId, now, amount, TransactionType::DEPOSIT, "Deposit - " + newId);
	transactionHistory_.push_back(depositTransaction);
	depositTransaction.process();
	cout << "Deposited: $" << amount << ", New Balance: $" << balance_ << endl;
}

// Withdraw from the account
// Creates a withdrawal transaction and updates the balance
void Account::withdraw(double amount)
{
	if (amount <= 0)
	{
		cerr << "Withdrawal amount must be positive." << endl;
		return;
	}
	if (amount > balance_)
	{
		cerr << "Insufficient funds for withdrawal." << endl;
		return;
	}
	balance_ -= amount;
	// Create a transaction record
	int newId = transactionHistory_.size() + 1; // Simple ID generation
	DateTime now; // Assume DateTime has a default constructor for current time
	Transaction withdrawalTransaction(newId, now, amount, TransactionType::WITHDRAWAL, "Withdrawal - " + newId);
	transactionHistory_.push_back(withdrawalTransaction);
	withdrawalTransaction.process();
	cout << "Withdrew: $" << amount << ", New Balance: $" << balance_ << endl;
}

double Account::getBalance() { return balance_; }
