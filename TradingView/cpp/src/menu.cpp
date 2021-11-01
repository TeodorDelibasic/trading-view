#include "menu.h"

void Menu::start() {
	CurrentUser& current = CurrentUser::getInstance();

	while (true) {
		try {
			std::size_t numberOfOptions = 0;

			if (current.getLoggedIn() == false) {
				this->notLoggedInMenu();
				numberOfOptions = 3;
			}
			else {
				this->loggedInMenu();
				numberOfOptions = 7;
			}

			this->inputOption();

			if (this->option == 0)
				this->option0();

			for (std::size_t i = 1; i < numberOfOptions; ++i) {
				if (i == this->option) {
					if (current.getLoggedIn() == false)
						this->notLoggedInOptions[i - 1]();
					else
						this->loggedInOptions[i - 1]();

					break;
				}
			}
		}
		catch (EndOfProgram& e) {
			std::cout << e.what() << std::endl;
			break;
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

void Menu::notLoggedInMenu() const {
	std::cout << "\nIzaberite jednu od sledece tri opcije unosom rednog broja opcije:\n";
	std::cout << "1. Ulogujte se\n";
	std::cout << "2. Registrujte se\n";
	std::cout << "0. Izlazak iz programa\n";
}

void Menu::loggedInMenu() const {
	std::cout << "\nIzaberite jednu od sledecih 7 opcije unosom rednog broja opcije:\n";
	std::cout << "1. Prikupljanje informacija o zeljenoj akciji u zeljenom intervalu\n";
	std::cout << "2. Prikaz informacija o zeljenoj akciji u zeljenom intervalu\n";
	std::cout << "3. Kupovina akcija\n";
	std::cout << "4. Prodaja akcija\n";
	std::cout << "5. Prikaz portfolija korisnika (trenutnog posedovanih akcija)\n";
	std::cout << "6. Izlogujte se\n";
	std::cout << "0. Izlazak iz programa\n";
}

void Menu::inputOption() {
	CurrentUser& current = CurrentUser::getInstance();

	std::cin >> this->option;
	if (current.getLoggedIn() == false && this->option > 2)
		throw IrregularOption();
	if (current.getLoggedIn() == true && this->option > 6)
		throw IrregularOption();
}

void Menu::option1NotLoggedIn() {
	std::string username;
	std::string password;

	std::cout << "Unesite korisnicko ime: ";
	std::cin >> username;

	std::cout << "Unesite sifru: ";
	std::cin >> password;

	User user(username, password);

	Database& db = Database::getInstance();

	if (db.checkUser(user) == true) {
		CurrentUser& current = CurrentUser::getInstance();
		current.logIn(user);
		std::cout << "Uspesno ste se ulogovali!\n";
	}
	else {
		throw Login();
	}
}

void Menu::option2NotLoggedIn() {
	std::string username;

	std::cout << "Unesite korisnicko ime: ";
	std::cin >> username;

	Database& db = Database::getInstance();

	if (db.checkUsername(username) == false) {
		std::string password;
		double balance;

		std::cout << "Unesite sifru: ";
		std::cin >> password;

		std::cout << "Unesite pocetni iznos na racunu: ";
		std::cin >> balance;

		if (balance <= 0)
			throw UserBalance();

		User user(username, password, balance);

		db.insertUser(user);

		CurrentUser& current = CurrentUser::getInstance();
		current.logIn(user);
	}
	else {
		throw Register();
	}
}

void Menu::option1LoggedIn() {
	std::string symbol;
	std::cout << "Unesite oznaku akcije koju zelite da kupite: ";  std::cin >> symbol;

	Interval interval = this->getInterval();
	StockInterval si(symbol, interval);

	StockExchange& wallStreet = StockExchange::getInstance();
	wallStreet.getInfoForStock(si);
}

void Menu::option2LoggedIn() {
	std::string symbol; 
	std::cout << "Unesite oznaku akcije koju zelite da kupite: ";  std::cin >> symbol;

	Interval interval = this->getInterval();
	StockInterval si(symbol, interval);

	StockExchange& wallStreet = StockExchange::getInstance();
	wallStreet.showInfoForStock(si);
}

void Menu::option3LoggedIn() {
	std::string symbol;
	int amount;

	std::cout << "Unesite simbol akcije: "; std::cin >> symbol;
	std::cout << "Unesite koliko akcija zelite da kupite: "; std::cin >> amount;

	StockExchange& wallStreet = StockExchange::getInstance();
	double price = wallStreet.getCurrentInfoForStock(symbol);

	CurrentUser& current = CurrentUser::getInstance();

	if (current.getUser().getBalance() < amount * price)
		throw InsufficientFunds();

	Database& db = Database::getInstance();

	if (db.checkIfStockExists(symbol) == false)
		db.addStock(symbol);

	int idUser = db.getUserId(current.getUser().getUsername());
	int idStock = db.getStockId(symbol);

	current.buying(amount * price);
	db.updateBalance(idUser, current.getUser().getBalance());

	if (db.checkIfOwnsExists(idUser, idStock) == false)
		db.addOwnsStock(idUser, idStock);

	db.insertTransaction(price, amount, idUser, idStock);
	int idTrans = db.lastTransId();
	db.insertBuying(idTrans, amount);
}

void Menu::option4LoggedIn() {
	int idBought;
	std::cout << "Unesite ID transakcije kojom ste kupili akcije koje zelite da prodate: "; std::cin >> idBought;

	Database& db = Database::getInstance();

	std::string symbol = db.getSymbol(idBought);

	int amount;
	std::cout << "Unesite koliko deonica zelite da prodate: "; std::cin >> amount;

	StockExchange& wallStreet = StockExchange::getInstance();
	double price = wallStreet.getCurrentInfoForStock(symbol);

	int left = db.checkHas(idBought, amount);
	if (left < 0)
		throw InsufficientShares();
	db.updateBuying(idBought, left);

	double profit = db.getProfit(idBought, price);
	
	CurrentUser& current = CurrentUser::getInstance();

	int idUser = db.getUserId(current.getUser().getUsername());
	int idStock = db.getStockId(symbol);

	current.selling(amount * price);
	db.updateBalance(idUser, current.getUser().getBalance());

	if (db.checkIfNotSold(idUser, idStock) == false)
		db.removeOwnsStock(idUser, idStock);

	db.insertTransaction(price, amount, idUser, idStock);
	int idTrans = db.lastTransId();
	db.insertSelling(idTrans, profit, idBought);
}

void Menu::option5LoggedIn() {
	CurrentUser& current = CurrentUser::getInstance();
	Database& db = Database::getInstance();
	int idUser = db.getUserId(current.getUser().getUsername());
}

void Menu::option6LoggedIn() {
	CurrentUser& current = CurrentUser::getInstance();
	current.logOut();
}

void Menu::option0() {
	throw EndOfProgram();
}

Interval Menu::getInterval() { 
	std::cout << "Izaberite na koji nacin unosite pocetno i krajnje vreme:\n";
	std::cout << "1. dd.mm.yyyy. hh:mm\n";
	std::cout << "2. Unix timestamp\n";

	std::size_t option;
	std::cin >> option;

	std::time_t times[2]{};

	for (std::size_t i = 0; i < 2; ++i) {
		if (i == 0)
			std::cout << "Unesite pocetno vreme: ";
		else
			std::cout << "Unesite krajnje vreme: ";

		std::time_t time = 0;
		if (option == 1) {
			std::string date; std::getline(std::cin, date);

			std::regex regex("(\\d\\d)\\.(\\d\\d)\\.(\\d\\d\\d\\d)\\. (\\d\\d):(\\d\\d)");
			std::smatch result;

			if (std::regex_match(date, result, regex)) {
				tm tm{};

				tm.tm_year = std::stoi(result.str(1)) - 1900;
				tm.tm_mon = std::stoi(result.str(2)) - 1;
				tm.tm_mday = std::stoi(result.str(3));

				tm.tm_hour = std::stoi(result.str(4));
				tm.tm_min = std::stoi(result.str(5));

				time = std::mktime(&tm);
			}
			else {
				throw IrregularDate();
			}
		}
		else if (option == 2) {
			std::cin >> time;

			std::time_t now = std::time(nullptr);
			if (time < 0 || time > now)
				throw IrregularTimestamp();
		}
		else {
			throw IrregularOption();
		}
		times[i] = time;
	}
	if (times[0] > times[1])
		throw IrregularInterval();

	return Interval(times[0], times[1]);
}
