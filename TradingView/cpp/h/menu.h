#ifndef _menu_h_
#define _menu_h_

#include <functional>
#include <vector>
#include <regex>
#include <ios>
#include <limits>

#include "database.h"
#include "currentUser.h"
#include "exceptions.h"
#include "stockInterval.h"
#include "stockExchange.h"

class Menu {

public:
	Menu() {
		this->notLoggedInOptions.push_back([this]() { this->option1NotLoggedIn(); });
		this->notLoggedInOptions.push_back([this]() { this->option2NotLoggedIn(); });

		this->loggedInOptions.push_back([this]() { this->option1LoggedIn(); });
		this->loggedInOptions.push_back([this]() { this->option2LoggedIn(); });
		this->loggedInOptions.push_back([this]() { this->option3LoggedIn(); });
		this->loggedInOptions.push_back([this]() { this->option4LoggedIn(); });
		this->loggedInOptions.push_back([this]() { this->option5LoggedIn(); });
		this->loggedInOptions.push_back([this]() { this->option6LoggedIn(); });
	}

	void start();

private:
	void notLoggedInMenu() const;
	void loggedInMenu() const;

	void inputOption(); // Unos opcije

	void option1NotLoggedIn(); // Login
	void option2NotLoggedIn(); // Register

	void option1LoggedIn(); // Prikupljanje informacija
	void option2LoggedIn(); // Prikaz informacija
	void option3LoggedIn(); // Kupovina
	void option4LoggedIn(); // Prodaja
	void option5LoggedIn(); // Porfolio
	void option6LoggedIn(); // Izlogovanje

	void option0();

	Interval getInterval();

	std::size_t option = 0;
	std::vector<std::function<void()>> notLoggedInOptions;
	std::vector<std::function<void()>> loggedInOptions;
};

#endif