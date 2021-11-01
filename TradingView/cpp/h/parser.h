#ifndef _parser_h_
#define _parser_h_

#include <string>
#include <vector>
#include <regex>
#include <map>
#include <iterator>
#include <functional>

#include "candlestick.h"
#include "crawler.h"

class Parser {

public:
	std::map<int, Candlestick> operator()(const std::string&); // Dohvatanje sveca
	std::vector<int> getTimestamps(const std::string&); // Dohvatanje timestampova
	void checkError(const std::string&); // Provera da li ima errora u dohvacenom sadrzaju

private:
	// Template metoda koja dohvata na osnovu regex patterna vektor vrednosti proizvoljnog tipa
	template<typename t>
	std::vector<t> getValues(const std::string& pattern, const std::string& json, std::function<t(std::string)>);
};

#endif