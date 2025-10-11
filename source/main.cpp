// Brandon Coesta 10/10/2025

#include "Account.h"
#include "Customer.h"
#include "Card.h"
#include "User.h"
#include <iostream>
#include <string>
#include <chrono>

using namespace std;
// in the future this will generate a PDF or other file format
// it will also get all this data from a database
// the time period will also be variable
void generateMiniStatement(const Account& account) {
	cout << "Mini Statement for Account: " << account.accountNumber() << endl;
	cout << "----------------------------------------" << endl;
	cout << "Date\t\t\tType\tAmount\tDescription" << endl;
	cout << "----------------------------------------" << endl;
	
	// Get current time and calculate 4 weeks ago
	auto now = std::chrono::system_clock::now();
	auto fourWeeksAgo = now - std::chrono::hours(24 * 28); // 28 days = 4 weeks
	
	for (const auto& transaction : account.transactionHistory()) {
		// Check if transaction date is within the last 4 weeks
		if(transaction.rawDate() >= fourWeeksAgo && transaction.rawDate() <= now) {
			cout << transaction.date() << "\t"
				 << (transaction.type() == TransactionType::DEPOSIT ? "Deposit" : "Withdrawal") << "\t"
				 << "$" << transaction.amount() << "\t"
				 << transaction.description() << endl;
		}
	}
	cout << "----------------------------------------" << endl;
	cout << "Current Balance: $" << account.getBalance() << endl;
	cout << "----------------------------------------" << endl;
}

string switchAccount(Customer& customer) {
	auto accountsList = customer.accounts();
	if (accountsList.empty()) {
		cout << "No accounts available." << endl;
		return "";
	}
	cout << "Select an account:" << endl;
	int accountNum = 1;
	for (const auto& account : accountsList) {
		cout << accountNum << ". " << account.accountNumber() << endl;
		accountNum++;
	}
	int choice = 0;
	cout << "Enter the number of the account to select: ";
	cin >> choice;
	if (choice < 1 || choice >= accountNum) {
		cout << "Invalid selection." << endl;
		return "";
	}
	auto it = accountsList.begin();
	advance(it, choice - 1);
	cout << "Switched to account: " << it->accountNumber() << endl;
	return it->accountNumber();
}

int main()
{
	string currentInput;

	// Dummy user and account for demonstration purposes
	// We will eventually pull this from a database
	User currentUser(1,"John Doe", "jd@gmail.com", "reallygoodpassword123", "705-671-7171");
	Customer currentCustomer(1);
	currentCustomer.setUser(currentUser);
	Account currentAccount("123456789", AccountType::CHEQUING);  // Changed to CHECKING
	currentCustomer.addAccount(currentAccount);

	cout << "-- GREATER SUDBURY BANKING SERVICE --" << endl;
	// display user information
	cout << "Welcome, " << currentUser.name() << "!" << endl;
	
	// show accounts - Fixed version
	auto accountsList = currentCustomer.accounts();
	int accountNum = 1;
	for (const auto& account : accountsList) {
		cout << "Account " << accountNum << ": " << account.accountNumber() << endl;
		accountNum++;
	}

	bool running = true;
	while(running)
	{
		cout << "Enter command (help for list of commands): ";
		getline(cin >> ws, currentInput);
		if(currentInput == "help")
		{
			cout << "Available commands:" << endl;
			cout << " help - Show this help message" << endl;
			cout << " view balance - Show selected account balance" << endl;
			cout << " view accounts - List all your accounts" << endl;
			cout << " view transactions - Show recent transactions for selected account" << endl;
			cout << " deposit - Deposit funds into selected account" << endl;
			cout << " withdraw - Withdraw funds from selected account" << endl;
			cout << " transfer - Transfer funds between accounts" << endl;
			cout << " new account - Open a new account" << endl;
			cout << " mini statement - Generate a mini statement for selected account" << endl;
			cout << " exit - Exit the application" << endl;
			// Add more commands as needed
		}
		else if(currentInput == "exit")
		{
			running = false;
			cout << "Exiting the application. Goodbye!" << endl;
		}
		else if(currentInput == "view balance")
		{
			// Placeholder for viewing balance logic
			cout << "Account balance: $" << currentAccount.getBalance() << endl;
		}
		else if (currentInput == "deposit") {
			cout << "How much would you like to deposit? ";
			double amnt = 0;  // Changed from int to double
			cin >> amnt;
			currentAccount.deposit(amnt);
		}
		else if (currentInput == "withdraw")
		{
			cout << "How much would you like to withdraw? ";
			double amnt = 0;  // Changed from int to double
			cin >> amnt;
			currentAccount.withdraw(amnt);
		}
		else if (currentInput == "transfer funds")
		{
			// Placeholder for transferring funds}
		}
		else if (currentInput == "view accounts")
		{
			auto accountsList = currentCustomer.accounts();
			int accountNum = 1;
			for (const auto& account : accountsList) {
				cout << "Account " << accountNum << ": " << account.accountNumber() << endl;
				accountNum++;
			}
		}
		else if (currentInput == "view transactions")
		{
			cout << "Recent transactions for account " << currentAccount.accountNumber() << ":" << endl;
			for (const auto& transaction : currentAccount.transactionHistory()) {
				cout << transaction.date() << " - " << (transaction.type() == TransactionType::DEPOSIT ? "Deposit" : "Withdrawal")
					 << ": $" << transaction.amount() << endl;
			}
		}
		else if (currentInput == "new account")
		{
			// Placeholder for opening a new account
			cout << "Feature not implemented yet." << endl;
		}
		else if (currentInput == "switch account")
		{
			string selectedAccountNumber = switchAccount(currentCustomer);
			if (!selectedAccountNumber.empty()) {
				// Find the account in the customer's accounts
				auto accountsList = currentCustomer.accounts();
				auto it = find_if(accountsList.begin(), accountsList.end(),
					[&selectedAccountNumber](const Account& account) {
						return account.accountNumber() == selectedAccountNumber;
					});
				if (it != accountsList.end()) {
					currentAccount = *it;
				}
			}
		}
		else if (currentInput == "mini statement")
		{
			generateMiniStatement(currentAccount);
		}
		else
		{
			cout << "Unknown command: " << currentInput << endl;
			cout << "Type 'help' for a list of available commands." << endl;
		}
	}

	return 0;
}
