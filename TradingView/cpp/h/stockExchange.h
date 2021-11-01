#ifndef _stockExchange_h_
#define _stockExchange_h_

#include <map>
#include <string>
#include <vector>
#include <ctime>

#include "exceptions.h"
#include "stock.h"
#include "candlestick.h"
#include "crawler.h"
#include "parser.h"
#include "stockInterval.h"

class StockExchange {

public:
	// Dohvatanje jedinog primerka berze
	static StockExchange& getInstance() {
		static StockExchange instance;
		return instance;
	}

	void getInfoForStock(const StockInterval& si); // Prikupljanje informacija o akciji u zeljenom intervalu
	void showInfoForStock(const StockInterval& si); // Prikaz informacija o akciji u zeljenom intervalu

	double getCurrentInfoForStock(const std::string& symbol); // Dohvatanje trenutne cene akcije

private:
	StockExchange() {}
	StockExchange(const StockExchange&) = delete;
	StockExchange(StockExchange&&) = delete;

	bool checkIfStockExists(const std::string&) const; // Provera da li se vec nalazi na berzi neka akcija

	std::map<std::string, Stock> stocks;
};


#endif