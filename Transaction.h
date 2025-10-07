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
	DateTime date() const;
	double amount() const;
	TransactionType type() const;
	string description() const;

	// processing and rollback would interact with accounts and database
	void process();
	void rollback();
private:
};

