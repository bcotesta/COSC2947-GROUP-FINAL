// Brandon Cotesta 10/08/2025
#include "User.h"

#include <iostream>

User::User(int id, string n, string e, string p, string ph): 
	userId_(id), name_(n), email_(e), passwordHash_(p), phone_(ph)
{
	// create user
}

int User::userId() const
{
	return userId_;
}
string User::name() const
{
	return name_;
}
string User::email() const
{
	return email_;
}
string User::phone() const
{
	return phone_;
}
string User::address() const
{
	return address_;
}
string User::passwordHash() const
{
	return passwordHash_;
}
bool User::login(string u, string p) const
{
	// Dummy implementation for illustration
	return (u == email_ && p == passwordHash_);
}
void User::logout() const
{
	// Dummy implementation for illustration
	cout << "User " << name_ << " logged out." << endl;
}
void User::updateProfile(string n, string e, string p, string ph)
{
	name_ = n;
	email_ = e;
	passwordHash_ = p;
	phone_ = ph;
}
// Note: address_ is not updated as it's not passed in the parameters


