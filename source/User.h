#pragma once
#include <string>
using namespace std;
class User
{
public:
	User(int id, string n, string e, string p, string ph);

	int userId() const;
	string name() const;
	string email() const;
	string phone() const;
	string address() const;
	string passwordHash() const;

	// functions
	bool login(string u, string p) const;
	void logout() const;

	/// <param name="name"></param>
	/// <param name="email"></param>
	/// <param name="password"></param>
	/// <param name="phone number"></param>
	void updateProfile(string n, string e, string p, string ph);
private:
	int userId_;
	string name_;
	string email_;
	string phone_;
	string address_;
	string passwordHash_;
};

