#pragma once
#include "Types.h"
#include <string>
using std::string;

class Account;

class Card {
public:
    Card(string cardNumber, CardType type, Account* linkedAccount);

    string cardNumber() const;
    CardType cardType() const;
    Account* linkedAccount() const;

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
    double creditLimit_;
};

