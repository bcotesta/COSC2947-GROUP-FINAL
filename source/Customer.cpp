#include "Customer.h"
#include <algorithm>
#include <stdexcept>

Customer::Customer(int userId) : userId_(userId), user_(nullptr)
{
}

Customer::Customer(const User& user) : userId_(user.userId()), user_(&user)
{
}

int Customer::getUserId() const
{
    return userId_;
}

const User* Customer::getUser() const
{
    return user_;
}

void Customer::setUser(const User& user)
{
    userId_ = user.userId();
    user_ = &user;
}

void Customer::addAccount(const Account& account)
{
    accounts_.push_back(account);
}

void Customer::removeAccount(const std::string& accountNumber)
{
    accounts_.remove_if([&accountNumber](const Account& account) {
        return account.accountNumber() == accountNumber;
    });
}

std::list<Account> Customer::accounts() const
{
	return accounts_;
}

std::list<Card> Customer::cards() const
{
	return cards_;
}

double Customer::viewBalance(std::string accountNumber) const
{
	// Find the account with the matching account number
	auto it = std::find_if(accounts_.begin(), accounts_.end(),
		[&accountNumber](const Account& account) {
			return account.accountNumber() == accountNumber;
		});
	
	if (it != accounts_.end()) {
		return it->getBalance();
	}
	
	// Account not found
	throw std::invalid_argument("Account not found: " + accountNumber);
}

void Customer::transferFunds(std::string fromAccount, std::string toAccount, double amount)
{
	if (amount <= 0) {
		throw std::invalid_argument("Transfer amount must be positive");
	}
	
	// Find source account
	auto fromIt = std::find_if(accounts_.begin(), accounts_.end(),
		[&fromAccount](Account& account) {
			return account.accountNumber() == fromAccount;
		});
	
	if (fromIt == accounts_.end()) {
		throw std::invalid_argument("Source account not found: " + fromAccount);
	}
	
	// Find destination account
	auto toIt = std::find_if(accounts_.begin(), accounts_.end(),
		[&toAccount](Account& account) {
			return account.accountNumber() == toAccount;
		});
	
	if (toIt == accounts_.end()) {
		throw std::invalid_argument("Destination account not found: " + toAccount);
	}
	
	// Check if source account has sufficient funds
	if (fromIt->getBalance() < amount) {
		throw std::invalid_argument("Insufficient funds in source account");
	}
	
	// Perform the transfer
	fromIt->withdraw(amount);
	toIt->deposit(amount);
}