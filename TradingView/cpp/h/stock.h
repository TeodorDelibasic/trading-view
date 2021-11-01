#ifndef _stock_h_
#define _stock_h_

#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>

#include "candlestick.h"
#include "crawler.h"
#include "parser.h"
#include "stockInterval.h"

class Stock {

public:
	Stock() {}
	Stock(const std::string& symbol) : candles() {
		this->symbol = symbol;
	}

	void getInfoForStock(const Interval& interval); // Prikupljanje informacija o akciji u intervalu
	void showInfoForStock(const Interval& interval); // Prikaz informacija o akciji u intervalu

	double getLastClose() const { // Dohvatanje trenutne vrednosti
		return std::prev(candles.end())->second.getClose();
	}

private:
	void addCandles(const std::map<int, Candlestick>& newCandles) { // Dodavanje sveca
		candles.insert(newCandles.begin(), newCandles.end());
	}

	bool checkIfStockContains(const Interval& interval); // Provera da li akcija sadrzi deonice u intervalu
	void printStock(const Interval& interval) const; // Stampanje akcije

	bool offerIndicators() const;

	std::string symbol;
	std::map<int, Candlestick> candles;

};

#endif