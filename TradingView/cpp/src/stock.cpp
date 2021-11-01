#include "stock.h"

void Stock::getInfoForStock(const Interval& interval) {
	if (!this->checkIfStockContains(interval)) {
		Crawler crawler;
		std::string json = crawler(Crawler::makeURL(StockInterval(this->symbol, interval)));

		Parser parser;
		this->addCandles(parser(json));
	}
}

void Stock::showInfoForStock(const Interval& interval) {
	this->getInfoForStock(interval);
	this->printStock(interval);
}

bool Stock::checkIfStockContains(const Interval& interval) {
	Crawler crawler;
	std::string json = crawler(Crawler::makeURL(StockInterval(this->symbol, interval)));

	Parser parser;
	parser.checkError(json);
	std::vector<int> timestamps = parser.getTimestamps(json);
	
	bool contains = std::all_of(timestamps.begin(), timestamps.end(), [this](int timestamp) {
		return this->candles.find(timestamp) != this->candles.end();
	});
	return contains;
}

void Stock::printStock(const Interval& interval) const {
	std::map<int, Candlestick> candlesMap;
	std::for_each(this->candles.begin(), this->candles.end(), [&](std::pair<int, Candlestick> candle) {
		if (candle.first >= interval.start && candle.first <= interval.end)
			candlesMap.insert(candle);
	});
	size_t maxWidth = 9;

	for (auto const& candle : candlesMap) {
		maxWidth = (maxWidth > std::to_string(candle.first).length()) ? maxWidth : std::to_string(candle.first).length();
		maxWidth = (maxWidth > candle.second.getMaxWidth()) ? maxWidth : candle.second.getMaxWidth();
	}

	int middle = (maxWidth * 5 + 4 - this->symbol.length()) / 2;
	std::string delimiter(maxWidth * 5 + 4, '-');

	std::cout << std::setw(middle) << " "; 
	std::cout << this->symbol;

	std::cout << std::endl << delimiter << std::endl;

	std::cout << std::setw(maxWidth) << "Time: ";
	std::cout << std::setw(maxWidth) << "Open: ";
	std::cout << std::setw(maxWidth) << "Close: ";
	std::cout << std::setw(maxWidth) << "High: ";
	std::cout << std::setw(maxWidth) << "Low: ";

	std::cout << std::endl << delimiter << std::endl;

	std::for_each(candlesMap.begin(), candlesMap.end(), [maxWidth](const std::pair<int, Candlestick>& candle) {
		std::cout << std::setw(maxWidth) << candle.first << " ";

		candle.second.printCandle(maxWidth);
		
		std::cout << std::endl;
	});
}

bool Stock::offerIndicators() const {
	int option;
	std::cout << "Da li zelite da Vam se prikazu indikatori?\n0 - Ne\n1 - Da" << std::endl;
	std::cin >> option;

	return option;
}
