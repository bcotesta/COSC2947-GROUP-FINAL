// Brandon Cotesta 10/06/2025

#include "Transaction.h"
#include <iostream>
using namespace std;

Transaction::Transaction(int id, DateTime date, double amount, TransactionType type, string description)
{
	// Initialize member variables
	// Eventually this data will be pulled from a database.
	this->id_ = id;
	this->date_ = date;
	this->amount_ = amount;
	this->type_ = type;
	this->description_ = description;
}

// Getters
int Transaction::id() const { return id_; }
DateTime Transaction::date() const { return date_; }
double Transaction::amount() const { return amount_; }
TransactionType Transaction::type() const { return type_; }
string Transaction::description() const { return description_; }

// Process the transaction (stub implementation)
void Transaction::process()
{
	// this would update account balances and record the transaction in a database
	cout << "Processing transaction ID: " << id_ << ", Amount: " << amount_ << endl;
}

void Transaction::rollback()
{
	// this would reverse the transaction effects on account balances and update the database
	cout << "Rolling back transaction ID: " << id_ << ", Amount: " << amount_ << endl;
}