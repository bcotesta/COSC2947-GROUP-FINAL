#pragma once
#include "Card.cpp"

using namespace std;
class Customer
{
public:
	list<Account> accounts() const;
	list<Card> cards() const;
	// functions
	double viewBalance(string accountNumber) const;
	/// <summary>
	/// Trandsfers funds between two accounts owned by the customer
	/// </summary>
	/// <param name="fromAccount"> ID for the account the money comes from</param>
	/// <param name="toAccount"> ID for the account the money comes out of</param>
	/// <param name="amount"></param>
	void transferFunds(string fromAccount, string toAccount, double amount);
private:
	list<Account> accounts_;
	list<Card> cards_;
};

