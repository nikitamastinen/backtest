#include <string>
#include <map>

#include "../lib/candle.hpp"
#include "../lib/candles.hpp"
#include "../lib/trade.hpp"

#include "strategy.hpp"

namespace backTest {

struct Proto {
  const std::string SHORT = "S";
  const std::string LONG = "L";
  const std::string UNSET = "N";
};

class BackTest {
 public:
  Proto proto;
  std::map<std::string, std::vector<lib::Candle>> data;

  std::string coin = "BTC";
  bool isReinvest = false;
  // double fee = 0.002;
  double fee = 0;

  std::vector<lib::Candle> candles;
  strategy::Strategy strategy;

  std::string position = proto.UNSET;

  double baseFundAtStart = 1000;
  double baseFund = 0;
  double targetFund = 0;
  double borrowedTargetFund = 0;

  double initialPrice = 0;

  std::vector<strategy::StrategyEvent>::iterator eventIterator;
  std::vector<strategy::TradeParams>::iterator optionsIterator;
  size_t candleIndex = 0;

 public:
  BackTest() {
    // data["BTC"] = lib::load_klines("4h");
  }

  void openPosition() {
    position = eventIterator->position;
    optionsIterator = eventIterator->options.begin();

    initialPrice = candles[candleIndex].price_close;
    //    std::cout << initialPrice << std::endl;
    double openFund = baseFund;
    if (!isReinvest) {
      openFund = std::min(baseFund, baseFundAtStart);
    }

    if (position == proto.LONG) {
      targetFund +=
          (openFund - openFund * fee) / candles[candleIndex].price_close;
      baseFund -= openFund;
    }

    if (position == proto.SHORT) {
      borrowedTargetFund += openFund / candles[candleIndex].price_close;
      baseFund += openFund - openFund * fee;
    }
  }

  void closePosition() {
    if (position == proto.LONG) {  // пока что закрываем на всю сумму
      double baseFundResult = targetFund * candles[candleIndex].price_close;
      baseFund += baseFundResult;
      baseFund -= baseFundResult * fee;

      targetFund = 0;
    }

    if (position == proto.SHORT) {  // пока что закрываем всю сумму
      double baseFundResult =
          borrowedTargetFund * candles[candleIndex].price_close;
      baseFund -= baseFundResult;
      baseFund -= baseFundResult * fee;

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
    if (optionsIterator == eventIterator->options.end()) {
      return false;
    }

    // std::cout << initialPrice << std::endl;
    if (position == proto.LONG &&
        candles[candleIndex].price_close <=
            optionsIterator->stopLoss * initialPrice) {
      return true;
    }
    if (position == proto.SHORT &&
        candles[candleIndex].price_close >=
            optionsIterator->stopLoss * initialPrice) {
      return true;
    }
    return false;
  }

  bool isTakeProfitReached() const {
    if (optionsIterator == eventIterator->options.end()) {
      return false;
    }
    if (position == "L")
      // std::cout << candles[candleIndex].price_close << ' ' <<
      // optionsIterator->takeProfit * initialPrice << ' ' <<
      // optionsIterator->takeProfit <<  std::endl;
      if (position == proto.LONG &&
          candles[candleIndex].price_close >=
              optionsIterator->takeProfit * initialPrice) {
        return true;
      }
    if (position == proto.SHORT &&
        candles[candleIndex].price_close <=
            optionsIterator->takeProfit * initialPrice) {
      return true;
    }
    return false;
  }

  bool isMoveTradeParamsCondition() const {
    if (optionsIterator == eventIterator->options.end()) {
      return false;
    }
    return isTakeProfitReached() &&
           optionsIterator + 1 != eventIterator->options.end();
  }

  bool isClosePositionCondition() const {
    if (isStopLossReached())
      std::cout << "sl";
    if (isTakeProfitReached())
      std::cout << "tp";
    if (isStopLossReached() ||
        (isTakeProfitReached() &&
         optionsIterator + 1 == eventIterator->options.end())) {
      return true;
    }
    return false;
  }

  bool isOpenPositionCondition() const {
    if (eventIterator == strategy.events.end()) {
      return false;
    }
    if (!isPositionOpened() &&
        eventIterator->timestamp <= candles[candleIndex].time_close) {
      return true;
    }
    return false;
  }

  std::vector<lib::Trade> test(const std::string& strategyAbsolutePath) {
    std::cout << "test started...\n";

    std::vector<lib::Trade> trades;
    strategy = strategy::Strategy(strategyAbsolutePath, "BTC");

    candleIndex = 0;
    baseFund = baseFundAtStart;
    eventIterator = strategy.events.begin();

    for (; candleIndex < candles.size(); ++candleIndex) {
      if (isClosePositionCondition()) {
        closePosition();

        // update trades
        trades.back().closeTime = candles[candleIndex].time_close;
        trades.back().fundAfter = baseFund;
        trades.back().reward =
            trades.back().fundAfter - trades.back().fundBefore;

        ++eventIterator;
        continue;
      }
      if (isOpenPositionCondition()) {
        // update trades
        trades.push_back({
            eventIterator->timestamp,
            0,
            eventIterator->position,
            baseFund,
            0,
            0,
        });

        openPosition();
        std::cout << baseFund << ' ' << targetFund << std::endl;
        continue;
      }
      if (isMoveTradeParamsCondition()) {
        // move stop-loss and take-profit
        moveTradeParams();
      }
    }

    if (isPositionOpened()) {
      closePosition();

      trades.back().closeTime = candles[candleIndex - 1].time_close;
      trades.back().fundAfter = baseFund;
      trades.back().reward = trades.back().fundAfter - trades.back().fundBefore;
    }
    return trades;
  }
};

}  // namespace backTest
