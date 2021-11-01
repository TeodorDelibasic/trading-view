#include "stockExchange.h"

bool StockExchange::checkIfStockExists(const std::string& symbol) const {
	return this->stocks.find(symbol) != this->stocks.end();
}

void StockExchange::getInfoForStock(const StockInterval& si) {
	if (!this->checkIfStockExists(si.symbol)) {
		Stock newStock(si.symbol); newStock.getInfoForStock(si.interval);
		this->stocks.insert({ si.symbol, newStock });
	}
	else
		this->stocks[si.symbol].getInfoForStock(si.interval);
}

void StockExchange::showInfoForStock(const StockInterval& si) {
	this->getInfoForStock(si);
	this->stocks[si.symbol].showInfoForStock(si.interval);
}

double StockExchange::getCurrentInfoForStock(const std::string& symbol) {
	std::time_t now = std::time(nullptr);
	std::time_t fiveDaysBefore = now - 432000;

	this->getInfoForStock(StockInterval(symbol, Interval(fiveDaysBefore, now)));

	return this->stocks[symbol].getLastClose();
}
