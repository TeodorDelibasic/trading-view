#include "parser.h"

std::map<int, Candlestick> Parser::operator()(const std::string& json) {
	std::vector<std::string> patterns;
	patterns.push_back("\"high\":\\[([^\\]]*)\\]");
	patterns.push_back("\"low\":\\[([^\\]]*)\\]");
	patterns.push_back("\"open\":\\[([^\\]]*)\\]");
	patterns.push_back("\"close\":\\[([^\\]]*)\\]");

	std::vector<std::vector<double>> values(patterns.size());
	std::transform(patterns.begin(), patterns.end(), values.begin(), [&json, this](const std::string& pattern) {
		return this->getValues<double>(pattern, json, [](std::string str) { return std::stod(str); });
	});

	std::vector<int> timestamps = this->getTimestamps(json);

    std::map<int, Candlestick> candles;

	for (size_t i = 0; i < values[0].size(); ++i) {
		int timestamp = timestamps[i];

		double high = values[0][i];
		double low = values[1][i];
		double open = values[2][i];
		double close = values[3][i];

		candles.insert({ timestamp, Candlestick(open, close, high, low) });
	}

    return candles;
}

std::vector<int> Parser::getTimestamps(const std::string& json) {
	return this->getValues<int>("\"timestamp\":\\[([^\\]]*)\\]", json, [](std::string str) { return std::stoi(str); });
}

void Parser::checkError(const std::string& json) {
	std::regex errorPattern("\"error\":null");
	std::smatch errorResult;

	if (!std::regex_search(json, errorResult, errorPattern)) {
		errorPattern = "\"description\":\"(.*)\"";
		std::regex_search(json, errorResult, errorPattern);

		throw JsonError(errorResult.str(1));
	}
}

template<typename t>
std::vector<t> Parser::getValues(const std::string& pattern, const std::string& json, std::function<t(std::string)> f) {
	std::regex regex(pattern);
	std::smatch result;
	std::regex_search(json, result, regex);

	std::vector<t> temp;
	std::string strResult = result.str(1);
	regex = "[^,]+";

	auto begin = std::sregex_iterator(strResult.begin(), strResult.end(), regex);
	auto end = std::sregex_iterator();

	for (std::sregex_iterator i = begin; i != end; ++i) {
		std::smatch match = *i;
		temp.push_back(f((*i).str()));
	}

	return temp;
}