#pragma once
#include <list>
#include <string>
#include "Account.h"
#include "Card.h"

class Customer
{
public:
    std::list<Account> accounts() const;
    std::list<Card> cards() const;

    double viewBalance(std::string accountNumber) const;
    void transferFunds(std::string fromAccount, std::string toAccount, double amount);

private:
    std::list<Account> accounts_;
    std::list<Card> cards_;
};

