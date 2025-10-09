// Brandon Cotesta 10/08/2025
#include "Card.h"

Card::Card(string cardNumber, CardType type, Account* linkedAccount)
{
	this->cardNumber_ = cardNumber;
	this->cardType_ = type;
	this->linkedAccount_ = linkedAccount;
	this->blocked_ = false; // Cards are unblocked by default
	if (type == CardType::CREDIT)
	{
		this->creditLimit_ = 5000.0; // Default credit limit
	}
	else
	{
		this->creditLimit_ = 0.0; // No credit limit for debit cards
	}
}

// Getters
string Card::cardNumber() const { return cardNumber_; }
CardType Card::cardType() const { return cardType_; }
Account* Card::linkedAccount() const { return linkedAccount_; }
bool Card::isBlocked() const { return blocked_; }

double Card::checkLimit() const
{
	if (cardType_ == CardType::CREDIT)
	{
		return creditLimit_;
	}
	else
	{
		return 0.0; // No credit limit for debit cards
	}
}

void Card::makePayment(double amount)
{
	if (blocked_)
	{
		cerr << "Card is blocked. Cannot make payment." << endl;
		return;
	}
	if (amount <= 0)
	{
		cerr << "Payment amount must be positive." << endl;
		return;
	}
	if (cardType_ == CardType::CREDIT)
	{
		if (amount > creditLimit_)
		{
			cerr << "Payment exceeds credit limit." << endl;
			return;
		}
		// For credit cards, we would typically just record the payment
		int newId = linkedAccount_->transactionHistory().size() + 1; // Simple ID generation
		DateTime now; // Assume DateTime has a default constructor for current time
		Transaction paymentTransaction(newId, now, amount, TransactionType::PAYMENT, "Credit Card Payment - " + to_string(newId));
		linkedAccount_->transactionHistory().push_back(paymentTransaction);
		paymentTransaction.process();
		cout << "Credit card payment of $" << amount << " made successfully." << endl;
	}
	else if (cardType_ == CardType::DEBIT)
	{
		if (amount > linkedAccount_->getBalance())
		{
			cerr << "Insufficient funds in linked account for debit card payment." << endl;
			return;
		}
		linkedAccount_->withdraw(amount);
		cout << "Debit card payment of $" << amount << " made successfully." << endl;
	}
}

void Card::block() { blocked_ = true; }
void Card::unblock() { blocked_ = false; }