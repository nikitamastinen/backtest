#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cstdlib>
#include "lib/candles.hpp"
#include <boost/json.hpp>
//#include "backtest/controller.hpp"

using std::string;
using std::unordered_map;
using std::vector;

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

typedef long long ll;

// struct Candle {
//   ll start;
//   ll end;
//
//   double open;
//   double high;
//   double low;
//   double close;
//
//   double volume;
//   double quote_volume;
//   double TBVBase;
//   double TBV_quote;
//   double ignore;
//
//   ll NOT;
//
//   explicit Candle(const string &line) {
//     auto currentLine = parse(line);
//
////    start = std::strtoll(currentLine[0].c_str(), nullptr, 0) / 1000;
////    open = std::strtod(currentLine[1].c_str(), nullptr);
////    high = std::strtod(currentLine[2].c_str(), nullptr);
////    low = std::strtod(currentLine[3].c_str(), nullptr);
////    close = std::strtod(currentLine[4].c_str(), nullptr);
////    volume = std::strtod(currentLine[5].c_str(), nullptr);
////
////    end = std::strtoll(currentLine[6].c_str(), nullptr, 0) / 1000;
////    quote_volume = std::stod(currentLine[7]);
////    NOT = std::stoll(currentLine[8]);
////    TBVBase = std::stod(currentLine[9]);
////    TBV_quote = std::stod(currentLine[10]);
////    ignore = std::stod(currentLine[11]);
//  }
//
//  static vector<string> parse(const string &line) {
//    string cur;
//    vector<string> res;
//    res.reserve(11);
//    for (char i: line) {
//      if (i == ',') {
//        res.push_back(cur);
//        cur = "";
//      } else {
//        cur += i;
//      }
//    }
//    res.push_back(cur);
//    return res;
//  }
//};
//
// const string timeframeMain = "4h";
// const string timeframeTrailing = "5m";
//
// unordered_map<string, unordered_map<string, vector<Candle>>> candles;
// vector<string> coins;
//
// namespace loader {
//  int startYear = 2022;
//  int startMonth = 1;
//  int currentYear = 2022;
//  int currentMonth = 6;
//
//  void load_coins(const string &absolutePath =
//  R"(D:\business\backtester\data\pairs.txt)") {
//    cout << "starting load_coins..." << std::endl;
//
//    ifstream pairsFD(absolutePath);
//    string line;
//    while (getline(pairsFD, line)) {
//      coins.push_back(line.substr(0, line.size() - 4));
//    }
//    pairsFD.close();
//  }
//
//  void loadCandles(const string &dataPath =
//  R"(D:\business\backtester\data\)") {
//    int ii = 0;
//    cout << "starting loadCandles..." << std::endl;
//
//    for (const auto &timeframe
//        : {timeframeMain, timeframeTrailing}) {
//      unordered_map<string, vector<Candle>> candlesTimeframe;
//      for (const auto &coin: coins) {
//        vector<Candle> candlesCoin;
//        vector<int> months;
//
//        for (int year = startYear; year <= currentYear; ++year) {
//          if (startYear < year && year < currentYear) {
//            for (int i = 1; i <= 12; ++i) months.push_back(i);
//          } else if (startYear == currentYear) {
//            for (int i = startMonth; i < currentMonth; ++i)
//            months.push_back(i);
//          } else if (year == currentYear) {
//            for (int i = 1; i < currentMonth; ++i) months.push_back(i);
//          } else {
//            for (int i = startMonth; i <= 12; ++i) months.push_back(i);
//          }
//
//          for (auto month: months) {
//            string last = std::to_string(month);
//            if (last.size() == 1) last = "0" + last;
//            string path =
//                dataPath + "\\" + coin + "USDT-" + timeframe + "-" +
//                std::to_string(year) + "-" + last + ".csv";
//
//            ifstream candlesFD(path);
//            string line;
//            while (candlesFD >> line) {
//              candlesCoin.emplace_back(line);
//              ii++;
//              if (ii % 100000 == 0)
//              cout << ii << endl;
//            }
//            candlesFD.close();
//          }
//        }
//        candlesTimeframe[coin] = std::move(candlesCoin);
//      }
//      candles[timeframe] = std::move(candlesTimeframe);
//    }
//    for (auto coin: coins) {
//      cout << candles[timeframeTrailing]["BTC"].size() << endl;
//    }
//  }
//}
//
// void run() {
//  std::ios_base::sync_with_stdio(false);
//  cin.tie(nullptr);
//  cout.tie(nullptr);
//
//  loader::load_coins();
//  loader::loadCandles();
//
//}

#include "rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <iostream>
//#include <boost/asio/io_service.hpp>
#include "backtest/strategy.hpp"
#include "backtest/backtest.hpp"

using namespace rapidjson;
// using namespace simdjson;

int main() {
  auto backTest = backTest::BackTest();
  backTest.candles = {lib::Candle{0, 100, 0, 0, 100, 0, 10},
      lib::Candle{10, 100, 0, 0, 100, 0, 20},
                      lib::Candle{20, 100, 0, 0, 95, 0, 30},
                      lib::Candle{30, 95, 0, 0, 90, 0, 40},
                      lib::Candle{40, 90, 0, 0, 91, 0, 50},
                      lib::Candle{50, 91, 0, 0, 92, 0, 60},
                      lib::Candle{60, 92, 0, 0, 95, 0, 70},
                      lib::Candle{70, 95, 0, 0, 110, 0, 80},
                      lib::Candle{80, 110, 0, 0, 105, 0, 90},
                       lib::Candle{90, 105, 0, 0, 95, 0, 100},
                       lib::Candle{100, 95, 0, 0, 90, 0, 110},
                       lib::Candle{110, 90, 0, 0, 95, 0, 120},
                       lib::Candle{120, 95, 0, 0, 90, 0, 130},
                       lib::Candle{130, 100, 0, 0, 95, 0, 140},
                       lib::Candle{140, 105, 0, 0, 100, 0, 150},
                       lib::Candle{150, 110, 0, 0, 105, 0, 160}};
  auto r =
      backTest.test(R"(D:\business\backtester\testData\simple_strategy.json)");
  for (auto to : r) {
    cout << to.toString() << std::endl << std::endl;
  }
  return 0;
}

//int main() {
//  controller::run();
//  return 0;
//}