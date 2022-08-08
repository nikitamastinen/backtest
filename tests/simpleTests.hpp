#include "../backtest/backtest.hpp"
#include <iostream>

using namespace std;

namespace tests {

void testFixedTrailingExitInTheEnd() {
  auto backTest = backTest::BackTest();
  backTest.candles = {lib::Candle{1, 100, 0, 0, 100, 0, 2},
                      lib::Candle{2, 100, 0, 0, 110, 0, 3},
                      lib::Candle{3, 110, 0, 0, 120, 0, 4},
                      lib::Candle{4, 120, 0, 0, 115, 0, 5}};
  auto r =
      backTest.test(R"(D:\business\backtester\testData\simple_strategy.json)");
  for (auto to : r) {
    cout << to.toString() << std::endl << std::endl;
  }
  /*
  [
    {
      "t": 1,
      "cl": false,
      "cs": false,
      "p": "L",
      "tl": 23600,
      "o": [
        {
          "t": 1.1,
          "s": 0.9,
          "f": false
        },
        {
          "t": 100000,
          "s": 0.95,
          "f": false
        }
      ]
    }
  ]
  */
}

void testFixedTrailingExitStopLoss() {
  auto backTest = backTest::BackTest();
  backTest.candles = {lib::Candle{1, 100, 0, 0, 100, 0, 2},
                      lib::Candle{2, 100, 0, 0, 110, 0, 3},
                      lib::Candle{3, 110, 0, 0, 120, 0, 4},
                      lib::Candle{4, 120, 0, 0, 114, 0, 5}};
  auto r =
      backTest.test(R"(D:\business\backtester\testData\simple_strategy.json)");
  for (auto to : r) {
    cout << to.toString() << std::endl << std::endl;
  }

  /*
  [
    {
      "t": 1,
      "cl": false,
      "cs": false,
      "p": "L",
      "tl": 23600,
      "o": [
        {
          "t": 1.1,
          "s": 0.9,
          "f": false
        },
        {
          "t": 100000,
          "s": 0.95,
          "f": false
        }
      ]
    }
  ]
  */
}

void testFixedShortExitStopLoss() {
  auto backTest = backTest::BackTest();

  backTest.candles = {lib::Candle{1654041600, 100, 0, 0, 100, 0, 1654056000},
                      lib::Candle{1654056000, 100, 0, 0, 107, 0, 1654070400},
                      lib::Candle{1654070400, 107, 0, 0, 60, 0, 1654084800},
                      lib::Candle{1654084800, 60, 0, 0, 110, 0, 1654099200}};
  auto r =
      backTest.test(R"(D:\business\backtester\testData\simple_strategy.json)");
  for (auto to : r) {
    cout << to.toString() << std::endl << std::endl;
  }

  /*
   [
    {
     "t": 1,
     "cl": false,
     "cs": false,
     "p": "S",
     "tl": 23600,
     "o": [
       {
         "t": 0.9,
         "s": 1.1,
         "f": false
       }
     ]
    }
  ]
  */
}

void testFixedLongExitStopLoss1() {
  auto backTest = backTest::BackTest();

  backTest.candles = {lib::Candle{1654041600, 100, 0, 0, 100, 0, 1654056000},
                      lib::Candle{1654056000, 100, 0, 0, 107, 0, 1654070400},
                      lib::Candle{1654070400, 107, 0, 0, 60, 0, 1654084800},
                      lib::Candle{1654084800, 60, 0, 0, 110, 0, 1654099200}};
  auto r =
      backTest.test(R"(D:\business\backtester\testData\simple_strategy.json)");
  for (auto to : r) {
    cout << to.toString() << std::endl << std::endl;
  }

  /*
   [
    {
     "t": 1654041600,
     "cl": false,
     "cs": false,
     "p": "L",
     "tl": 23600,
     "o": [
       {
         "t": 1.1,
         "s": 0.9,
         "f": false
       }
     ]
    }
  ]
  */
}

void testFixedLongExitStopLoss2() {
  auto backTest = backTest::BackTest();
  backTest.candles = {lib::Candle{1654041600, 100, 0, 0, 100, 0, 1654056000},
                      lib::Candle{1654056000, 100, 0, 0, 110, 0, 1654070400},
                      lib::Candle{1654070400, 110, 0, 0, 60, 0, 1654084800},
                      lib::Candle{1654084800, 60, 0, 0, 110, 0, 1654099200}};
  auto r =
      backTest.test(R"(D:\business\backtester\testData\simple_strategy.json)");
  for (auto to : r) {
    cout << to.toString() << std::endl << std::endl;
  }

  /*
   [
    {
     "t": 1654041600,
     "cl": false,
     "cs": false,
     "p": "L",
     "tl": 23600,
     "o": [
       {
         "t": 1.1,
         "s": 0.9,
         "f": false
       }
     ]
    }
  ]
  */
}

void testFixedShortExitStopLoss2Trades() {
  auto backTest = backTest::BackTest();
  backTest.candles = {lib::Candle{1, 100, 0, 0, 100, 0, 2},
                      lib::Candle{2, 100, 0, 0, 110, 0, 3},
                      lib::Candle{3, 110, 0, 0, 60, 0, 4},
                      lib::Candle{4, 60, 0, 0, 110, 0, 5},
                      lib::Candle{5, 100, 0, 0, 100, 0, 6},
                      lib::Candle{6, 100, 0, 0, 110, 0, 7},
                      lib::Candle{7, 110, 0, 0, 60, 0, 8},
                      lib::Candle{8, 60, 0, 0, 110, 0, 9}};
  auto r =
      backTest.test(R"(D:\business\backtester\testData\simple_strategy.json)");
  for (auto to : r) {
    cout << to.toString() << std::endl << std::endl;
  }

  /*
   [
    {
     "t": 1,
     "cl": false,
     "cs": false,
     "p": "S",
     "tl": 23600,
     "o": [
       {
         "t": 0.9,
         "s": 1.1,
         "f": false
       }
     ]
    },
    {
     "t": 5,
     "cl": false,
     "cs": false,
     "p": "S",
     "tl": 23600,
     "o": [
       {
         "t": 0.9,
         "s": 1.1,
         "f": false
       }
     ]
    }
  ]
  */
}

void testFixedTradesIntersectionLong() {
  auto backTest = backTest::BackTest();
  backTest.candles = {lib::Candle{0, 100, 0, 0, 100, 0, 1},
                      lib::Candle{1, 100, 0, 0, 100, 0, 2},
                      lib::Candle{2, 100, 0, 0, 111, 0, 3},
                      lib::Candle{3, 111, 0, 0, 60, 0, 4},
                      lib::Candle{4, 60, 0, 0, 110, 0, 5},
                      lib::Candle{5, 110, 0, 0, 102, 0, 6},
                      lib::Candle{6, 102, 0, 0, 121, 0, 7},
                      lib::Candle{7, 121, 0, 0, 60, 0, 8},
                      lib::Candle{8, 60, 0, 0, 110, 0, 9}};
  auto r =
      backTest.test(R"(D:\business\backtester\testData\simple_strategy.json)");
  for (auto to : r) {
    cout << to.toString() << std::endl << std::endl;
  }

  /*
  [
    {
      "t": 1,
      "cl": false,
      "cs": false,
      "p": "L",
      "tl": 23600,
      "o": [
        {
          "t": 1.1,
          "s": 0.9,
          "f": false
        }
      ]
    },
    {
      "t": 1,
      "cl": false,
      "cs": false,
      "p": "L",
      "tl": 23600,
      "o": [
        {
          "t": 1.1,
          "s": 0.9,
          "f": false
        }
      ]
    }
  ]
  */
}

void testNotFixedLong() {
  auto backTest = backTest::BackTest();
  backTest.candles = {lib::Candle{0, 100, 0, 0, 100, 0, 1},
                      lib::Candle{1, 100, 0, 0, 100, 0, 2},
                      lib::Candle{2, 100, 0, 0, 105, 0, 3},
                      lib::Candle{3, 105, 0, 0, 110, 0, 4},
                      lib::Candle{4, 110, 0, 0, 115, 0, 5},
                      lib::Candle{5, 115, 0, 0, 120, 0, 6},
                      lib::Candle{6, 120, 0, 0, 115, 0, 7},
                      lib::Candle{7, 115, 0, 0, 110, 0, 8},
                      lib::Candle{8, 110, 0, 0, 100, 0, 9}};
  auto r =
      backTest.test(R"(D:\business\backtester\testData\simple_strategy.json)");
  /*
  [
    {
      "t": 1,
      "cl": false,
      "cs": false,
      "p": "L",
      "tl": 23600,
      "o": [
        {
          "t": 100,
          "s": 0.9,
          "f": false
        }
      ]
    }
  ]
   */
}

}  // namespace tests