#include <string>
#include <map>
#include "../lib/candle.hpp"

namespace lib {

class BackTest {
 public:
  std::map<std::string, std::vector<Candle>> candles;
  std::vector<std::string> coins;

 public:
  BackTest(const std::string& coin, const std::vector<Candle>& candles) {

  }

  void test() {

  }
};

}  // namespace lib
