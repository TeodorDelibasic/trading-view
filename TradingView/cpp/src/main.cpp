#include "database.h"
#include "stockExchange.h"
#include "currentUser.h"
#include "menu.h"

int main(void) {
	Database& db = Database::getInstance();
	StockExchange& wallStreet = StockExchange::getInstance();
	CurrentUser& current = CurrentUser::getInstance();
	
	Menu m;
	m.start();
}