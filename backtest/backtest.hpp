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
  std::map<std::string, std::vector<lib::Candle>> data;

  std::string coin = "BTC";
  bool reinvest = false;
  double fee = 0.002;

  std::vector<lib::Candle> candles;
  strategy::Strategy strategy;

  std::string position = "N";
  double baseFund = 0;
  double targetFund = 0;
  double borrowedTargetFund = 0;

  std::vector<strategy::StrategyEvent>::iterator eventIterator;
  std::vector<strategy::TradeParams>::iterator optionsIterator;

  size_t candleIndex = 0;

 public:
  BackTest() {
    data["BTC"] = lib::load_klines("4h");
  }

  void openPosition() {
    position = eventIterator->position;
    optionsIterator = eventIterator->options.begin();

    if (position == proto.LONG) {
      targetFund += baseFund / candles[candleIndex].price_close;
      targetFund -= baseFund * fee;

      baseFund = 0;
    }

    if (position == proto.SHORT) {
      borrowedTargetFund += baseFund / candles[candleIndex].price_close;
      borrowedTargetFund -= baseFund * fee;

      baseFund += baseFund;
    }
  }

  void closePosition() {
    if (position == proto.LONG) {  // пока что закрываем на всю сумму
      double baseResult = targetFund * candles[candleIndex].price_close;
      baseFund += baseResult;
      baseFund -= baseResult * fee;

      targetFund = 0;
    }

    if (position == proto.SHORT) {  // пока что закрываем всю сумму
      double baseResult = borrowedTargetFund * candles[candleIndex].price_close;
      baseFund -= baseResult;
      baseFund -= baseResult * fee;

      borrowedTargetFund = 0;
    }

    position = proto.UNSET;
  }

  bool moveTradeParams() {
    if (optionsIterator == eventIterator->options.end()) {
      return false;
    }
    ++optionsIterator;
    return true;
  }

  bool isPositionOpened() const {
    return position != proto.UNSET;
  }

  bool isStopLossReached() const {
    if (position == proto.LONG &&
        candles[candleIndex].price_close <= optionsIterator->stopLoss) {
      return true;
    }
    if (position == proto.SHORT &&
        candles[candleIndex].price_close >= optionsIterator->stopLoss) {
      return true;
    }
    return false;
  }

  bool isTakeProfitReached() const {
    if (position == proto.LONG &&
        candles[candleIndex].price_close >= optionsIterator->takeProfit) {
      return true;
    }
    if (position == proto.SHORT &&
        candles[candleIndex].price_close <= optionsIterator->takeProfit) {
      return true;
    }
    return false;
  }

  bool isClosePositionCondition() const {
    if (isStopLossReached() ||
        (isTakeProfitReached() &&
         optionsIterator + 1 == eventIterator->options.end())) {
      return true;
    }
    return false;
  }

  void test(const std::string& strategyAbsolutePath) {
    std::cout << "test started...\n";

    candles = data["BTC"];
    strategy = strategy::Strategy(strategyAbsolutePath, "BTC");


    for (eventIterator = strategy.events.begin(); eventIterator != strategy.events.end(); ++eventIterator) {
      for (candleIndex = 0; candleIndex < candles.size(); ++candleIndex) {
        if (eventIterator->timestamp <= candles[candleIndex].time_close) {

        }
      }
    }
  }
};

}  // namespace backTest
