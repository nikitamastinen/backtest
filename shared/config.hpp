#pragma once

#include <string>
#include <iostream>

namespace shared {

class Config {
  public:
  static Config* instance;

 private:
  std::string configPath;
  std::string baseAsset = "USDT";
  std::string dataDirectoryPath = R"(D:\business\backtester\data\)";
  std::string pairsFilePath = R"(D:\business\backtester\data\pairs.txt)";

  void update() {
  }

 public:
  std::string getBaseAsset() {
    return baseAsset;
  }

  std::string getPairsFilePath() {
    return pairsFilePath;
  }

  std::string getDataDirectoryPath() {
    return dataDirectoryPath;
  }

  static Config& getInstance() {
    std::cout << "config constructed\n";
    if (instance == nullptr) {
      instance = new Config();
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


}  // namespace shared