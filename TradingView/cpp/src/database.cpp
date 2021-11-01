#include "database.h"

bool Database::checkUser(User& user) {
	std::string query = "SELECT Balance FROM User WHERE Username = ? AND Password = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_text(this->sqlStmt, 1, user.getUsername().c_str(), user.getUsername().length(), SQLITE_TRANSIENT);
	sqlite3_bind_text(this->sqlStmt, 2, user.getPassword().c_str(), user.getPassword().length(), SQLITE_TRANSIENT);

	this->rc = sqlite3_step(this->sqlStmt);

	if (this->rc == SQLITE_DONE) {
		return false;
	}

	double balance = sqlite3_column_double(this->sqlStmt, 0);
	user.setBalance(balance);

	sqlite3_finalize(this->sqlStmt);
	return true;
}

bool Database::checkUsername(const std::string& username) {
	std::string query = "SELECT Username FROM User WHERE Username = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_text(this->sqlStmt, 1, username.c_str(), username.length(), SQLITE_TRANSIENT);

	this->rc = sqlite3_step(this->sqlStmt);

	if (this->rc == SQLITE_DONE) {
		return false;
	}

	sqlite3_finalize(this->sqlStmt);
	return true;
}

void Database::insertUser(const User& u) {

	std::string query = "INSERT INTO User(Username, Password, Balance) VALUES(?, ?, ?)";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_text(this->sqlStmt, 1, u.getUsername().c_str(), u.getUsername().length(), SQLITE_TRANSIENT);
	sqlite3_bind_text(this->sqlStmt, 2, u.getPassword().c_str(), u.getPassword().length(), SQLITE_TRANSIENT);
	sqlite3_bind_double(this->sqlStmt, 3, u.getBalance());

	sqlite3_step(this->sqlStmt);
	sqlite3_finalize(this->sqlStmt);
}

int Database::getUserId(const std::string& username) {
	std::string query = "SELECT IdUser FROM User WHERE Username = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_text(this->sqlStmt, 1, username.c_str(), username.length(), SQLITE_TRANSIENT);

	sqlite3_step(this->sqlStmt);
	int id = sqlite3_column_int(this->sqlStmt, 0);
	sqlite3_finalize(this->sqlStmt);

	return id;
}

int Database::getStockId(const std::string& symbol) {
	std::string query = "SELECT IdStock FROM Stock WHERE Symbol = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_text(this->sqlStmt, 1, symbol.c_str(), symbol.length(), SQLITE_TRANSIENT);

	sqlite3_step(this->sqlStmt);
	int id = sqlite3_column_int(this->sqlStmt, 0);
	sqlite3_finalize(this->sqlStmt);

	return id;
}

bool Database::checkIfStockExists(const std::string& symbol) {
	std::string query = "SELECT * FROM Stock WHERE Symbol = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_text(this->sqlStmt, 1, symbol.c_str(), symbol.length(), SQLITE_TRANSIENT);

	this->rc = sqlite3_step(this->sqlStmt);

	if (this->rc == SQLITE_DONE) {
		return false;
	}

	sqlite3_finalize(this->sqlStmt);
	return true;
}

void Database::addStock(const std::string& symbol) {
	std::string query = "INSERT INTO Stock(Symbol) VALUES(?)";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_text(this->sqlStmt, 1, symbol.c_str(), symbol.length(), SQLITE_TRANSIENT);

	sqlite3_step(this->sqlStmt);
	sqlite3_finalize(this->sqlStmt);
}

bool Database::checkIfOwnsExists(int idUser, int idStock) {
	std::string query = "SELECT * FROM OwnsStock\
						 WHERE IdStock = ? AND IdUser = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, idStock);
	sqlite3_bind_int(this->sqlStmt, 2, idUser);

	this->rc = sqlite3_step(this->sqlStmt);

	if (this->rc == SQLITE_DONE) {
		return false;
	}

	sqlite3_finalize(this->sqlStmt);
	return true;
}

void Database::addOwnsStock(int IdUser, int IdStock) {
	std::string query = "INSERT INTO OwnsStock(IdUser, IdStock) VALUES(?, ?)";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, IdUser);
	sqlite3_bind_int(this->sqlStmt, 2, IdStock);

	sqlite3_step(this->sqlStmt);
	sqlite3_finalize(this->sqlStmt);
}

void Database::insertBuying(int idTrans, int amount) {
	std::string query = "INSERT INTO Buying(IdTrans, AmountLeft) VALUES(?, ?)";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, idTrans);
	sqlite3_bind_int(this->sqlStmt, 2, amount);

	sqlite3_step(this->sqlStmt);
	sqlite3_finalize(this->sqlStmt);
}

std::string Database::getSymbol(int idTrans) {
	std::string query = "SELECT s.Symbol FROM Trans t, Stock s WHERE t.IdTrans = ? AND t.IdStock = s.IdStock";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, idTrans);

	this->rc = sqlite3_step(this->sqlStmt);
	if (rc != SQLITE_ROW)
		return "";

	std::string symbol = reinterpret_cast<const char*>(sqlite3_column_text(this->sqlStmt, 0));
	sqlite3_finalize(this->sqlStmt);

	return symbol;
}

int Database::checkHas(int idBought, int amount) {
	std::string query = "SELECT AmountLeft FROM Buying WHERE IdTrans = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, idBought);

	sqlite3_step(this->sqlStmt);
	int left = sqlite3_column_int(this->sqlStmt, 0);
	sqlite3_finalize(this->sqlStmt);

	return left - amount;
}

double Database::getProfit(int idBought, double price) {
	std::string query = "SELECT PricePerStock FROM Trans WHERE IdTrans = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, idBought);

	sqlite3_step(this->sqlStmt);
	double ppp = sqlite3_column_double(this->sqlStmt, 0);
	sqlite3_finalize(this->sqlStmt);

	return price - ppp;
}

void Database::updateBuying(int idBought, int amount) {
	std::string query = "UPDATE Buying SET AmountLeft = ? WHERE IdTrans = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, amount);
	sqlite3_bind_int(this->sqlStmt, 2, idBought);

	sqlite3_step(this->sqlStmt);
	sqlite3_finalize(this->sqlStmt);
}

bool Database::checkIfNotSold(int idUser, int idStock) {
	std::string query = "SELECT * FROM Buying b, Trans t\
						 WHERE b.IdTrans = t.IdTrans AND t.IdUser = ? AND t.IdStock = ? AND b.AmountLeft > 0";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, idUser);
	sqlite3_bind_int(this->sqlStmt, 2, idStock);

	this->rc = sqlite3_step(this->sqlStmt);

	if (this->rc == SQLITE_DONE) {
		return false;
	}

	sqlite3_finalize(this->sqlStmt);
	return true;
}

void Database::removeOwnsStock(int IdUser, int IdStock) {
	std::string query = "DELETE FROM OwnsStock WHERE IdUser = ? AND IdStock = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, IdUser);
	sqlite3_bind_int(this->sqlStmt, 2, IdStock);

	sqlite3_step(this->sqlStmt);
	sqlite3_finalize(this->sqlStmt);
}

void Database::insertSelling(int idTrans, double profit, int idBought) {
	std::string query = "INSERT INTO Selling(IdTrans, ProfitPerStock, IdBought) VALUES(?, ?, ?)";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, idTrans);
	sqlite3_bind_double(this->sqlStmt, 2, profit);
	sqlite3_bind_int(this->sqlStmt, 3, idBought);

	sqlite3_step(this->sqlStmt);
	sqlite3_finalize(this->sqlStmt);
}

int Database::lastTransId() {
	return (int)sqlite3_last_insert_rowid(this->db);
}

void Database::insertTransaction(double price, int amount, int idUser, int idStock) {
	std::string query = "INSERT INTO Trans(PricePerStock, Amount, IdUser, IdStock) VALUES(?, ?, ?, ?)";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_double(this->sqlStmt, 1, price);
	sqlite3_bind_int(this->sqlStmt, 2, amount);
	sqlite3_bind_int(this->sqlStmt, 3, idUser);
	sqlite3_bind_int(this->sqlStmt, 4,idStock);

	sqlite3_step(this->sqlStmt);
	sqlite3_finalize(this->sqlStmt);
}

void Database::updateBalance(int idUser, double balance) {
	std::string query = "UPDATE User SET Balance = ? WHERE IdUser = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_double(this->sqlStmt, 1, balance);
	sqlite3_bind_int(this->sqlStmt, 2, idUser);

	sqlite3_step(this->sqlStmt);
	sqlite3_finalize(this->sqlStmt);
}

void Database::printTable() {

	std::string query = "SELECT * FROM User";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	while (sqlite3_step(this->sqlStmt) == SQLITE_ROW) {

		for(int i = 0; i < sqlite3_data_count(this->sqlStmt); ++i){

			switch (sqlite3_column_type(this->sqlStmt, i)) {

			case SQLITE_INTEGER:
				std::cout << sqlite3_column_int(this->sqlStmt, i);
				break;
			case SQLITE_FLOAT:
				std::cout << sqlite3_column_double(this->sqlStmt, i);
				break;
			case SQLITE_TEXT:
				std::cout << sqlite3_column_text(this->sqlStmt, i);
				break;
			}

			if (i < sqlite3_data_count(this->sqlStmt) - 1)
				std::cout << " ";
		}
	}
	std::cout << std::endl;
	sqlite3_finalize(sqlStmt);
}

double Database::getBalance(int idUser) {
	std::string query = "SELECT Balance FROM User WHERE IdUser = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, idUser);

	sqlite3_step(this->sqlStmt);
	double balance = sqlite3_column_double(this->sqlStmt, 0);
	sqlite3_finalize(this->sqlStmt);

	return balance;
}

std::string Database::getOwnedStocks(int idUser) {
	std::string query = "SELECT Symbol FROM OwnsStock o, Stock s WHERE s.IdStock = o.IdStock AND o.IdUser = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, idUser);

	StockExchange& se = StockExchange::getInstance();

	std::ostringstream stringStream;

	int id = 0;
	while (sqlite3_step(this->sqlStmt) == SQLITE_ROW) {
		std::string symbol = reinterpret_cast<const char*>(sqlite3_column_text(this->sqlStmt, 0));
		double price = se.getCurrentInfoForStock(symbol);

		stringStream << ++id << ". " << symbol << " " << price << "\n";
	}
	sqlite3_finalize(sqlStmt);

	return stringStream.str();
}

std::string Database::getBought(int idUser) {
	std::string query = "SELECT t.IdTrans, s.Symbol, t.PricePerStock, t.Amount, b.AmountLeft\
						 FROM Trans t, Buying b, Stock s\
						 WHERE s.IdStock = t.IdStock AND t.IdTrans = b.IdTrans AND t.IdUser = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, idUser);

	StockExchange& se = StockExchange::getInstance();

	std::ostringstream stringStream;

	while (sqlite3_step(this->sqlStmt) == SQLITE_ROW) {
		int idTrans = sqlite3_column_int(this->sqlStmt, 0);
		std::string symbol = reinterpret_cast<const char*>(sqlite3_column_text(this->sqlStmt, 1));

		double ppp = sqlite3_column_double(this->sqlStmt, 2);
		double price = se.getCurrentInfoForStock(symbol);

		int amount1 = sqlite3_column_int(this->sqlStmt, 3);
		int amount2 = sqlite3_column_int(this->sqlStmt, 4);

		stringStream << idTrans << ". " << symbol << " " << amount1 << " " << amount2 << " "
			<< ppp << " " << price << "\n";
	}
	sqlite3_finalize(sqlStmt);

	return stringStream.str();
}

std::string Database::getSold(int idUser) {
	std::string query = "SELECT t.IdTrans, st.Symbol, t.PricePerStock, t.Amount, se.ProfitPerStock\
						 FROM Trans t, Selling se, Stock st\
						 WHERE st.IdStock = t.IdStock AND t.IdTrans = se.IdTrans AND t.IdUser = ?";

	this->rc = sqlite3_prepare_v2(this->db, query.c_str(), 255, &(this->sqlStmt), nullptr);
	this->checkError();

	sqlite3_bind_int(this->sqlStmt, 1, idUser);

	std::ostringstream stringStream;

	while ((this->rc = sqlite3_step(this->sqlStmt)) == SQLITE_ROW) {
		int idTrans = sqlite3_column_int(this->sqlStmt, 0);
		std::string symbol = reinterpret_cast<const char*>(sqlite3_column_text(this->sqlStmt, 1));

		double price = sqlite3_column_double(this->sqlStmt, 2);
		double profit = sqlite3_column_double(this->sqlStmt, 4);

		int amount = sqlite3_column_int(this->sqlStmt, 3);

		stringStream << idTrans << ". " << symbol << " " << amount << " " << price << " " << profit << "\n";
	}
	this->checkError();
	sqlite3_finalize(sqlStmt);

	return stringStream.str();
}

void Database::checkError() {
	if (this->rc != SQLITE_OK && this->rc != SQLITE_DONE) {
		sqlite3_finalize(this->sqlStmt);
		this->sqlStmt = nullptr;
		std::cout << sqlite3_errmsg(this->db);
		throw std::exception(sqlite3_errmsg(this->db));
	}
}

void Database::execStmt(const std::string& query) {

	this->rc = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
	this->checkError();
	this->sqlStmt = nullptr;
}

void Database::createTables() {
	this->createTableUser();
	this->createTableStock();
	this->createTableTransaction();
	this->createTableBuying();
	this->createTableSelling();
	this->createTableOwnsStock();
}

void Database::createTableUser() {
	std::string query = "CREATE TABLE IF NOT EXISTS User(\
						IdUser INTEGER,\
						Username CHAR(20) NOT NULL UNIQUE,\
						Password CHAR(20) NOT NULL,\
						Balance REAL NOT NULL CHECK(Balance > 0),\
						PRIMARY KEY(IdUser))";

	this->execStmt(query);
}

void Database::createTableStock() {
	std::string query = "CREATE TABLE IF NOT EXISTS Stock(\
						IdStock INTEGER,\
						Symbol CHAR(20) NOT NULL UNIQUE,\
						PRIMARY KEY(IdStock))";

	this->execStmt(query);
}

void Database::createTableTransaction() {
	std::string query = "CREATE TABLE IF NOT EXISTS Trans(\
						IdTrans INTEGER,\
						PricePerStock REAL NOT NULL CHECK(PricePerStock > 0),\
						Amount INTEGER NOT NULL CHECK(Amount > 0),\
						IdUser INTEGER NOT NULL,\
						IdStock INTEGER NOT NULL,\
						PRIMARY KEY(IdTrans),\
						FOREIGN KEY(IdUser) REFERENCES User(IdUser),\
						FOREIGN KEY(IdStock) REFERENCES Stock(IdStock))";

	this->execStmt(query);
}

void Database::createTableBuying() {
	std::string query = "CREATE TABLE IF NOT EXISTS Buying(\
						IdTrans INTEGER,\
						AmountLeft INTEGER NOT NULL CHECK(AmountLeft >= 0),\
						PRIMARY KEY(IdTrans),\
						FOREIGN KEY(IdTrans) REFERENCES Trans(IdTrans))";

	this->execStmt(query);
}

void Database::createTableSelling() {
	std::string query = "CREATE TABLE IF NOT EXISTS Selling(\
						IdTrans INTEGER,\
						ProfitPerStock REAL NOT NULL,\
						IdBought INTEGER,\
						PRIMARY KEY(IdTrans),\
						FOREIGN KEY(IdTrans) REFERENCES Trans(IdTrans),\
						FOREIGN KEY(IdBought) REFERENCES Buying(IdTrans))";

	this->execStmt(query);
}

void Database::createTableOwnsStock() {
	std::string query = "CREATE TABLE IF NOT EXISTS OwnsStock(\
						IdUser INTEGER,\
						IdStock INTEGER,\
						PRIMARY KEY(IdUser, IdStock),\
						FOREIGN KEY(IdUser) REFERENCES User(IdUser),\
						FOREIGN KEY(IdStock) REFERENCES Stock(IdStock))";

	this->execStmt(query);
}