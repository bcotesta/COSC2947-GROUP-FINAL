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
	bool running = true;
	while(running)
	{
		cout << "Enter command (help for list of commands): ";
		getline(cin, currentInput);
		if(currentInput == "help")
		{
			cout << "Available commands:" << endl;
			cout << " help - Show this help message" << endl;
			cout << " exit - Exit the application" << endl;
			// Add more commands as needed
		}
		else if(currentInput == "exit")
		{
			running = false;
			cout << "Exiting the application. Goodbye!" << endl;
		}
		else
		{
			cout << "Unknown command: " << currentInput << endl;
			cout << "Type 'help' for a list of available commands." << endl;
		}
	}

	return 0;
}
