#pragma once
#include <string>
#include "Types.h"
#include <Account.cpp>
using namespace std;

class Card
{
public:
	Card(string cardNumber, CardType type, Account* linkedAccount);
	string cardNumber() const;
	CardType cardType() const;
	Account* linkedAccount() const;
	// functions
	void makePayment(double amount);
	
	double checkLimit() const;
	void block();
	void unblock();
	bool isBlocked() const;
private:
	string cardNumber_;
	CardType cardType_;
	Account* linkedAccount_; // Pointer to the linked account
	bool blocked_;
	double creditLimit_; // Applicable for credit cards
};

