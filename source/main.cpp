#include "Account.h"
#include "Customer.h"
#include "Card.h"
#include <iostream>
#include <string>

using namespace std;
int main()
{
	cout << "-- GREATER SUDBURY BANKING SERVICE --" << endl;
	string currentInput;
	Account currentAccount("123456789", AccountType::CHEQUING);
	currentAccount.deposit(1000.0); // Initial deposit for demonstration
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
			cout << " deposit - Deposit funds into selected account" << endl;
			cout << " withdraw - Withdraw funds from selected account" << endl;
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
			int amnt = 0;
			cin >> amnt;
			currentAccount.deposit(amnt);
		}
		else if (currentInput == "withdraw")
		{
			// Placeholder for withdrawal logic
			cout << "How much would you like to withdraw? ";
			int amnt = 0;
			cin >> amnt;
			currentAccount.withdraw(amnt);
		}
		else if (currentInput == "transfer funds")
		{
			// Placeholder for transferring funds}
		}
		else
		{
			cout << "Unknown command: " << currentInput << endl;
			cout << "Type 'help' for a list of available commands." << endl;
		}
	}

	return 0;
}
