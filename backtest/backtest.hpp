#pragma once

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
  const double EPS = 1e-11;

  Proto proto;
  std::map<std::string, std::vector<lib::Candle>> data;

  std::string coin = "BTC";
  bool isReinvest = true;
  double fee = 0.002;
  // double fee = 0;

  std::vector<lib::Candle> candles;
  strategy::Strategy strategy;
  std::vector<lib::Trade> trades;

  std::string position = proto.UNSET;

  double baseFundAtStart = 100;
  double baseFund = 0;
  double targetFund = 0;
  double borrowedTargetFund = 0;

  double initialPrice = 0;

  double upperPrice = 0;
  double lowerPrice = 0;

  std::vector<strategy::StrategyEvent>::iterator eventIterator;
  std::vector<strategy::StrategyEvent>::iterator closeEventIterator;

  std::vector<strategy::TradeParams>::iterator optionsIterator;

  size_t candleIndex = 0;

 public:
  explicit BackTest() {
    // data["BTC"] = lib::load_klines("4h");
  }

  void openPosition() {
    // update trades
    trades.push_back({
        candles[candleIndex].time_close,
        0,
        eventIterator->position,
        0,
        0,
        trades.empty() ? 0 : trades.back().reward,
    });

    position = eventIterator->position;
    optionsIterator = eventIterator->options.begin();

    initialPrice = candles[candleIndex].price_close;
    upperPrice = initialPrice;
    lowerPrice = initialPrice;

    if (!isReinvest && baseFund > baseFundAtStart) {
      trades.back().reward += baseFund - baseFundAtStart;
      baseFund = baseFundAtStart;
    }

    if (position == proto.LONG) {
      trades.back().fundBefore = baseFund;
      targetFund +=
          (baseFund - baseFund * fee) / candles[candleIndex].price_close;
      baseFund = 0;
    }

    if (position == proto.SHORT) {
      trades.back().fundBefore = baseFund;

      borrowedTargetFund += baseFund / candles[candleIndex].price_close;
      baseFund += baseFund - baseFund * fee;
    }
  }

  void closePosition() {
    if (position == proto.LONG) {  // пока что закрываем на всю сумму
      double baseFundResult = targetFund * candles[candleIndex].price_close;
      baseFundResult -= baseFundResult * fee;
      baseFund += baseFundResult;

      trades.back().fundAfter = baseFund;
      targetFund = 0;
    }

    if (position == proto.SHORT) {  // пока что закрываем всю сумму
      double baseFundResult =
          borrowedTargetFund * candles[candleIndex].price_close;
      baseFundResult += baseFundResult * fee;
      baseFund -= baseFundResult;

      trades.back().fundAfter = baseFund;
      borrowedTargetFund = 0;
    }

    // update trades
    trades.back().closeTime = candles[candleIndex].time_close;
    trades.back().reward += trades.back().fundAfter - trades.back().fundBefore;
    position = proto.UNSET;
  }

  bool moveTradeParams() {
    if (optionsIterator == eventIterator->options.end()) {
      return false;
    }
    ++optionsIterator;
    /*
    Uncomment if you want to calculate the trailing relative to the new price,
    otherwise everything is relative to the initial price.
    */
    // initialPrice = candles[candleIndex].price_close;
    return true;
  }

  bool isPositionOpened() const {
    return position != proto.UNSET;
  }

  bool isStopLossReached() const {
    if (position == proto.UNSET ||
        optionsIterator == eventIterator->options.end()) {
      return false;
    }
    std::cerr << optionsIterator->isFixed << ' ' << upperPrice << std::endl;
    if (position == proto.LONG &&
        candles[candleIndex].price_close - EPS <=
            (optionsIterator->isFixed ? 0 : upperPrice - initialPrice) +
                optionsIterator->stopLoss * initialPrice) {
      return true;
    }
    if (position == proto.SHORT &&
        candles[candleIndex].price_close + EPS >=
            (optionsIterator->isFixed ? 0 : lowerPrice - initialPrice) +
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
      if (position == proto.LONG &&
          candles[candleIndex].price_close + EPS >=
              optionsIterator->takeProfit * initialPrice) {
        return true;
      }
    if (position == proto.SHORT &&
        candles[candleIndex].price_close - EPS <=
            optionsIterator->takeProfit * initialPrice) {
      return true;
    }
    return false;
  }

  bool isMoveTradeParamsCondition() const {
    //    if (eventIterator->closeShort || eventIterator->closeLong) {
    //      return false;
    //    }
    if (optionsIterator == eventIterator->options.end()) {
      return false;
    }
    return isTakeProfitReached() &&
           optionsIterator + 1 != eventIterator->options.end();
  }

  bool isClosePositionCondition() const {
    //    if (position == proto.SHORT && eventIterator->closeShort) {
    //      return true;
    //    }
    //    if (position == proto.LONG && eventIterator->closeLong) {
    //      return true;
    //    }
    if (isStopLossReached() ||
        (isTakeProfitReached() &&
         optionsIterator + 1 == eventIterator->options.end())) {
      return true;
    }
    return false;
  }

  bool isOpenPositionCondition() const {
    //    if (eventIterator == strategy.events.end() || eventIterator->closeLong
    //    ||
    //        eventIterator->closeShort) {
    //      return false;
    //    }
    if (!isPositionOpened() &&
        eventIterator->timestamp <= candles[candleIndex].time_close &&
        eventIterator->timestamp >= candles[candleIndex].time_open) {
      return true;
    }
    return false;
  }

  std::vector<lib::Trade> test(const std::string& strategyAbsolutePath) {
    std::cout << "test started...\n";

    strategy = strategy::Strategy(strategyAbsolutePath, "BTC");

    candleIndex = 0;
    baseFund = baseFundAtStart;
    eventIterator = strategy.events.begin();
    closeEventIterator = strategy.events.begin();

    trades.clear();

    for (; candleIndex < candles.size(); ++candleIndex) {
      upperPrice = std::max(upperPrice, candles[candleIndex].price_close);
      lowerPrice = std::min(lowerPrice, candles[candleIndex].price_close);

      while (closeEventIterator != strategy.events.end() &&
             closeEventIterator->timestamp < candles[candleIndex].time_open &&
             !closeEventIterator->closeLong &&
             !closeEventIterator->closeShort) {
        ++closeEventIterator;
      }

      if (closeEventIterator != strategy.events.end() &&
          closeEventIterator->timestamp <= candles[candleIndex].time_close) {
        if (closeEventIterator->closeShort && position == proto.SHORT) {
          std::cout << candles[candleIndex].time_open << ' ' <<  closeEventIterator->timestamp << endl;
          closePosition();
          ++closeEventIterator;
        } else if (closeEventIterator->closeLong && position == proto.LONG) {
          closePosition();
          ++closeEventIterator;
        }
      }

      while (eventIterator != strategy.events.end() &&
             (eventIterator->timestamp < candles[candleIndex].time_open &&
                  !isPositionOpened() ||
              eventIterator->closeShort || eventIterator->closeLong)) {
        ++eventIterator;
      }

      if (isClosePositionCondition() ||
          (candleIndex + 1 == candles.size() && isPositionOpened())) {
        closePosition();
        ++eventIterator;
      }
      if (eventIterator == strategy.events.end()) {
        continue;
      }
      if (isOpenPositionCondition()) {
        openPosition();
        continue;
      }
      if (isMoveTradeParamsCondition()) {
        // move stop-loss and take-profit
        moveTradeParams();
      }
    }
    return trades;
  }
};

}  // namespace backTest
