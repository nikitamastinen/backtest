#include <string>
#include <limits>
#include <string>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include "candle.hpp"

namespace lib {

//struct Candle() {
//  void init(coin) {
//  }
//}

std::vector<lib::Candle> load_klines(const std::string& timeframe);

}  // namespace lib
