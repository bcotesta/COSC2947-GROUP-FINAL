// Brandon Cotesta 10/06/2025

#pragma once
#include "Types.h"
#include <string>
#include <memory>
using namespace std;
class Transaction
{
public:
	Transaction(int id, DateTime date, double amount, TransactionType type, string description);

	int id() const;
	double amount() const;
	TransactionType type() const;
	string date() const;
	DateTime rawDate() const;
	string description() const;

	// processing and rollback would interact with accounts and database
	void process();
	void rollback();
private:
	int id_;					// Unique transaction ID
	DateTime date_;				// Date and time of the transaction
	double amount_;				// Transaction amount
	TransactionType type_;		// Type of transaction
	string description_;		// Description or memo for the transaction
};

