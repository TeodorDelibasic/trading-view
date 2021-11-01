#ifndef _stockInterval_h_
#define _stockInterval_h_

#include <string>
#include <chrono>

struct Interval {
    std::time_t start;
    std::time_t end;

    Interval(std::time_t start, std::time_t end) {
        this->start = start;
        this->end = end;
    }

    std::string startString() const {
        std::stringstream ss;
        ss << this->start;
        return ss.str();
    }

    std::string endString() const {
        std::stringstream ss;
        ss << this->end;
        return ss.str();
    }
};

struct StockInterval {
    Interval interval;
    std::string symbol;

    StockInterval(const std::string& symbol, const Interval& interval) : interval(interval) {
        this->symbol = symbol;
    }

    std::string startString() const {
        return this->interval.startString();
    }

    std::string endString() const {
        return this->interval.endString();
    }
};

#endif