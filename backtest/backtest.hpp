#include <string>
#include <map>
#include "../lib/candle.hpp"
#include "../lib/candles.hpp"
#include "strategy.hpp"

namespace backTest {

struct Proto {
  std::string SHORT = "S";
  std::string LONG = "L";
  std::string UNSET = "N";
};

class BackTest {
 public:
  Proto proto;

  std::map<std::string, std::vector<lib::Candle>> candles;

  std::string position = "N";

  strategy::StrategyEvent event;
  std::vector<strategy::TradeParams>::iterator optionsIterator;

 public:
  BackTest() {
    candles["BTC"] = lib::load_klines("4h");
  }

  void openShort(const strategy::StrategyEvent& e) {
    position = proto.SHORT;
    event = e;
  }

  void moveStopLoss() {

  }

  void closePosition() {

  }

  bool isPositionOpened() {

  }

  bool isPositionClosed() {

  }

  bool isStopLossReached() {

  }

  bool isTakeProfitReached() {

  }

  void test(const std::string& strategyAbsolutePath) {
    std::cout << "test started...\n";
    auto strategy = strategy::Strategy(strategyAbsolutePath, "BTC");

    strategy.print();

    for (int e = 0; e < strategy.events.size(); ++e) {
      for (int c = 0; c < candles[strategy.coin].size(); ++c) {
        if (isPositionClosed()) {
          openPosition();
        }
      }
    }
  }
};

}  // namespace lib
