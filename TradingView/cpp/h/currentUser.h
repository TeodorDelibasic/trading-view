#ifndef _currentUser_h_
#define _currentUser_h_

#include "user.h"

class CurrentUser {

public:
	static CurrentUser& getInstance() {
		static CurrentUser instance;
		return instance;
	}

	bool getLoggedIn() const {
		return this->loggedIn;
	}
	User getUser() const {
		return *(this->current);
	}

	void buying(double price) {
		this->current->buying(price);
	}
	void selling(double price) {
		this->current->selling(price);
	}

	void logIn(const User& user) {
		this->loggedIn = true;

		current = new User(user);
	}
	void logOut() {
		this->loggedIn = false;

		if (current != nullptr)
			delete current;

		current = nullptr;
	}

	~CurrentUser() {
		if (current != nullptr)
			delete current;
	}

private:
	CurrentUser() {
		this->current = nullptr;
		this->loggedIn = false;
	}
	CurrentUser(const CurrentUser&) = delete;
	CurrentUser(CurrentUser&&) = delete;

	User* current;
	bool loggedIn;
};

#endif