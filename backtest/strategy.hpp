#pragma once

#include <ctime>
#include <utility>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include "rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <iostream>

#include "../shared/config.hpp"

namespace strategy {

struct TradeParams {
  double takeProfit;  // t
  double stopLoss;    // s
  bool isFixed;       // f
};

struct StrategyEvent {
 public:
  std::time_t timestamp{};  // t

  bool closeLong = false;   // cl
  bool closeShort = false;  // cs

  std::string position;  // p

  std::time_t timeLimit{0};  // tl

  std::vector<TradeParams> options;  // o
 public:
  [[nodiscard]] std::string toString() const {
    std::string result = "timestamp: " + std::to_string(timestamp);
    result += "\ncloseLong: " + std::to_string(closeLong);
    result += "\ncloseShort: " + std::to_string(closeShort);
    result += "\nposition: " + position;
    result += "\ntimeLimit: " + std::to_string(timeLimit);
    result += "\noptions:\n";
    for (const auto& to : options) {
      result += "{tp: " + std::to_string(to.takeProfit) + ',';
      result += " sl: " + std::to_string(to.stopLoss) + ',';
      result += " fixed: " + std::to_string(to.isFixed);
      result += "}, ";
    }
    result.pop_back();

    return result;
  }

  [[maybe_unused]] void print() const {
    std::cout << toString() << std::endl;
  }
};

struct Strategy {
  std::vector<StrategyEvent> events;
  std::string coin = "BTC";


  Strategy() = default;

  explicit Strategy(const std::string& absolutePath) {
    std::ifstream ifs{absolutePath};
    if (!ifs.is_open()) {
      throw std::runtime_error("can't open file with strategy");
    }
    rapidjson::IStreamWrapper isw{ifs};
    rapidjson::Document document;

    document.ParseStream(isw);

    const rapidjson::Value& d = document["strategy"];

    {
      if (document.HasMember("coin")) {
        coin = document["coin"].GetString();
      } else {
        std::cout << "strategy: coin doesn't set, default BTC\n";
      }
    }

    for (rapidjson::Value::ConstValueIterator e = d.Begin(); e != d.End();
         ++e) {
      StrategyEvent strategyEvent;

      strategyEvent.timestamp = static_cast<std::time_t>((*e)["t"].GetInt64());
      strategyEvent.closeShort = (*e)["cs"].GetBool();
      strategyEvent.closeLong = (*e)["cl"].GetBool();
      strategyEvent.position = (*e)["p"].GetString();
      //strategyEvent.timeLimit = static_cast<std::time_t>((*e)["tl"].GetInt64());

      std::vector<TradeParams> options;
      rapidjson::GenericArray<
          true, rapidjson::GenericValue<rapidjson::UTF8<>>::ValueType>
          arr = (*e)["o"].GetArray();
      for (rapidjson::Value::ConstValueIterator param = arr.Begin();
           param != arr.End(); ++param) {
        options.push_back({(*param)["t"].GetDouble(), (*param)["s"].GetDouble(),
                           (*param)["f"].GetBool()});
      }
      strategyEvent.options = std::move(options);
      events.push_back(std::move(strategyEvent));
    }
  }

  [[nodiscard]] std::string toString() const {
    std::string result = "coin: " + coin + "\n";
    result += "events:\n";
    for (const auto& to : events) {
      result += to.toString() + "\n\n";
    }
    return result;
  }

  [[maybe_unused]] void print() const {
    std::cout << toString() << std::endl;
  }
};

}  // namespace strategy