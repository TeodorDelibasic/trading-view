#ifndef _crawler_h_
#define _crawler_h_

#include <iostream>
#define CURL_STATICLIB

#include <curl/curl.h>
#include <string>

#include "parser.h"
#include "stockInterval.h"

class Crawler {

public:
	static std::string makeURL(const StockInterval& si); // Pomocna fja za pravljenje kompletnog URL-a

	std::string operator()(const std::string& url);

private:
	static size_t WriteCallback(void*, size_t, size_t, void*);

	static const std::string baseURL; // URL u koji se umece interval i oznaka akcije

};

#endif