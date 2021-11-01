#include "crawler.h"

const std::string Crawler::baseURL = "https://query1.finance.yahoo.com/v8/finance/chart/?period1=&period2=&interval=1h";

std::string Crawler::makeURL(const StockInterval& si) {
	int pos = 50;
	std::string url = Crawler::baseURL;

	url.insert(pos, si.symbol); pos += si.symbol.length() + 9;
	url.insert(pos, si.interval.startString()); pos += si.interval.startString().length() + 9;
	url.insert(pos, si.interval.endString());

	return url;
}

size_t Crawler::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string Crawler::operator()(const std::string& url) {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
	}

	return readBuffer;
}
