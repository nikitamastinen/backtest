#pragma once

#include <string>
#include <iostream>
#include "rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <fstream>

namespace shared {

class Config {
 public:
  static Config* instance;

 public:
  std::string configPath;

  std::string baseAsset = "USDT";
  std::string timeframe = "5m";

  std::time_t startYear = 2022;
  std::time_t startMonth = 1;
  std::time_t endYear = 2022;
  std::time_t endMonth = 5;

  double baseFundAtStart = 1000;

  double fee = 0.002;

  bool isReinvest = false;

  std::string dataDirectoryPath = R"(D:\business\backtester\data\)";
  std::string pairsFilePath = R"(D:\business\backtester\data\pairs.txt)";

  void update() {
    std::ifstream ifs{configPath};
    if (!ifs.is_open()) {
      throw std::runtime_error("can't open file with strategy");
    }
    rapidjson::IStreamWrapper isw{ifs};
    rapidjson::Document document;
    document.ParseStream(isw);

    baseAsset = document["baseAsset"].GetString();
    timeframe = document["timeframe"].GetString();

    startYear = static_cast<std::time_t>(document["startYear"].GetUint64());
    startMonth = static_cast<std::time_t>(document["startMonth"].GetUint64());
    endYear = static_cast<std::time_t>(document["endYear"].GetUint64());
    endMonth = static_cast<std::time_t>(document["endMonth"].GetUint64());

    baseFundAtStart = document["baseFundAtStart"].GetDouble();

    fee = document["fee"].GetDouble();
    isReinvest = document["isReinvest"].GetBool();

    dataDirectoryPath = document["dataDirectoryPath"].GetString();
    pairsFilePath = document["pairsFilePath"].GetString();
  }

  static Config& getInstance(const std::string& configPath = "") {
    if (instance == nullptr) {
      std::cout << "config constructed\n";
      instance = new Config();
      instance->configPath = configPath;
      instance->update();
    } else {
      instance->update();
    }
    return *instance;
  }

  ~Config() {
    std::cout << "config destructed\n";
    delete instance;
  }
};

void registerConfig(const std::string& configPath);

}  // namespace shared