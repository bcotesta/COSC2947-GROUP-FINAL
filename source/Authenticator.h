// Brandon Cotesta - 0456970
#pragma once
#include <string>
#include <iostream>
using namespace std;
class Authenticator
{
public:
	Authenticator();
	bool verifyCredentials(const string& username, const string& password);
	void resetPassword(const string& username);
private:
	string validUsername_; // Placeholder for valid username
	string validPassword_; // Placeholder for valid password
};

