#ifndef _candlestick_h_
#define _candlestick_h_

#include <algorithm>
#include <sstream>
#include <iostream>
#include <exception>
#include <sstream>
#include <vector>
#include <iomanip>

#include "exceptions.h"

// FINISHED!!!
class Candlestick {

public:
	Candlestick(double open, double close, double high, double low) {
		if (!(open >= 0 && close >= 0 && high >= 0 && low >= 0))
			throw CandlestickError();

		this->open = open;
		this->close = close;
		this->high = high;
		this->low = low;
	}

	double getClose() const {
		return this->close;
	}

	size_t getMaxWidth() const;

	void printCandle(int width) const;

private:
	double open, close;
	double high, low;
};

#endif