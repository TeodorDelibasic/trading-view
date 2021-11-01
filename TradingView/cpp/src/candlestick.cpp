#include "candlestick.h"

void Candlestick::printCandle(int width) const {
    std::ostringstream conv;
    conv << width;
    std::string w = conv.str();

    std::string raw = "%.3f %.3f %.3f %.3f";
    
    raw.insert(16, w);
    raw.insert(11, w);
    raw.insert(6, w);
    raw.insert(1, w);

    printf(raw.c_str(), this->open, this->close, this->high, this->low);
}

size_t Candlestick::getMaxWidth() const {
    std::vector<double> doubles{ this->open, this->close, this->high, this->low };

    size_t maxWidth = 0;
    for (auto const& x : doubles) {
        std::ostringstream strs;
        strs << x;
        std::string str = strs.str();

        int pos = (str.find('.') > -1) ? str.find('.') : str.length();
        str = str.substr(pos);

        maxWidth = (maxWidth < str.length()) ? str.length() : maxWidth;
    }

    return maxWidth;
}
