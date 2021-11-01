#ifndef _database_h_
#define _database_h_

#include <iostream>
#include <string>
#include <exception>
#include <sstream>
#include <stdlib.h>

#include "stockExchange.h"
#include "sqlite3.h"
#include "user.h"

class Database {

public:
	// -------- Dohvatanje jedine instance objekta za konekciju na bazu ---------
	static Database& getInstance() {
		static Database instance;
		return instance;
	}
	// -----------------------------------------------------------------------

	// -------- Metode u vezi sa korisnicima -----------
	bool checkUser(User& user); // Provera da li postoje korisnicko ime i sifra (Logovanje)
	bool checkUsername(const std::string& username); // Provera da li postoji korisnicko ime
	void insertUser(const User& user); // Umetanje novog korisnika (Registrovanje)
	// -------------------------------------------------

	// -------- Metode u vezi sa transakcijama -----------
	int getUserId(const std::string& username); // Dohvatanje UserId za username
	int getStockId(const std::string& symbol); // Dohvatanje StockId za symbol

	bool checkIfStockExists(const std::string& symbol); // Provera da li postoji akcija u bazi
	void addStock(const std::string& symbol); // Dodavanje akcije u bazu
	bool checkIfOwnsExists(int idUser, int idStock); // Provera da li poseduje akciju
	void addOwnsStock(int IdUser, int IdTrans); // Korisnik poseduje akciju
	void insertBuying(int idTrans, int amount); // Nova kupovina

	std::string getSymbol(int idTrans); // Dohvatanje simbola akcije za zadati ID transakcije
	int checkHas(int idBought, int amount); // Provera da li ima dovoljno deonica kupljenih sa trazenim id
	double getProfit(int idBought, double price); // Dohvatanje profita po jednoj deonici
	void updateBuying(int idBought, int amount); // Oduzimanje kolicine prodatih deonica iz kupljenih
	bool checkIfNotSold(int idUser, int idStock); // Provera da li korisnik vise ne poseduje akciju
	void removeOwnsStock(int idUser, int idStock); // Korisnik vise ne poseduje neku akciju
	void insertSelling(int idTrans, double profit, int idBought); // Nova prodaja

	int lastTransId(); // Poslednja umetnuta transakcija
	void insertTransaction(double price, int amount, int idUser, int idStock);
	void updateBalance(int idUser, double balance);
	// ---------------------------------------------------

	// -------- Portfolio ---------
	double getBalance(int idUser);
	std::string getOwnedStocks(int idUser);
	std::string getBought(int idUser);
	std::string getSold(int idUser);
	// --------------------------------
		
	void printTable();

	~Database() {
		sqlite3_close(this->db);
	}

private:

	// ---- Pomocne SQL wrapper metode ----
	void checkError();
	void execStmt(const std::string&);
	// ------------------------------------

	// ---- Kreiranje tabela ----
	void createTables();

	void createTableUser();
	void createTableStock();
	void createTableTransaction();
	void createTableBuying();
	void createTableSelling();
	void createTableOwnsStock();
	// --------------------------

	// --------------- Singleton -------------------
	Database() {
		this->rc = sqlite3_open("Berza.db", &db);
		this->checkError();

		this->createTables();
	}
	Database(const Database&) = delete;
	Database(Database&&) = delete;
	// ---------------------------------------------

	// -------- Atributi ---------
	sqlite3* db = nullptr;
	int rc;
	sqlite3_stmt* sqlStmt;
	// ---------------------------
};

#endif