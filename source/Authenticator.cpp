#include "Authenticator.h"

string validUsername = "user";
string validPassword = "password123";

Authenticator::Authenticator() {
	// this will initialize any needed resources related to the database for usernames and such
	// so like if(db exists){
	// 		connect to db
	//      get valid usernames and passwords
	//      store them in a suitable data structure
	//}
}

bool Authenticator::verifyCredentials(const string& username, const string& password) 
{
	return (username == validUsername && password == validPassword);
}

void Authenticator::resetPassword(const string& username) 
{
	// In a real application, this would involve more secure steps
	if (username == validUsername) {
		cout << "Password reset link has been sent to your registered email." << endl;
	} else {
		cout << "Username not found." << endl;
	}
}