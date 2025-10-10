#pragma once
#include <list>
#include <string>
#include "Account.h"
#include "Card.h"
#include "User.h"

class Customer
{
public:
    Customer(int userId);
    Customer(const User& user);
    
    int getUserId() const;
    const User* getUser() const;
    void setUser(const User& user);

    std::list<Account> accounts() const;
    std::list<Card> cards() const;

    double viewBalance(std::string accountNumber) const;
    void transferFunds(std::string fromAccount, std::string toAccount, double amount);
    
    void addAccount(const Account& account);
    void removeAccount(const std::string& accountNumber);

private:
    int userId_;  // Link to User
    const User* user_;  // Optional direct reference
    std::list<Account> accounts_;
    std::list<Card> cards_;
};

