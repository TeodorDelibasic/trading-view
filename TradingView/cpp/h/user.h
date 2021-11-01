#ifndef _user_h_
#define _user_h_

#include <iostream>
#include <string>

#include "exceptions.h"

// FINISHED!!!
class User {

public:
	User(const std::string& username, const std::string& password, double balance)
		: username(username), password(password), balance(balance) 
	{ }

	User(const std::string& username, const std::string& password) : User(username, password, 0) {}

	std::string getUsername() const { return this->username; }
	std::string getPassword() const { return this->password; }
	double getBalance() const { return this->balance; }

	void setBalance(double balance) { this->balance = balance; }
	void buying(double price) {
		this->balance -= price;
	}
	void selling(double price) {
		this->buying(-price);
	}

	friend std::ostream& operator<<(std::ostream& os, const User& user) {
		return os << user.username << " " << user.password << " " << user.balance;
	}

private:
	std::string username;
	std::string password;
	double balance;
};

#endif