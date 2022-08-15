#pragma once

#include <vector>
#include "../lib/trade.hpp"
#include "../lib/datetime.hpp"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"  // FileReadStream
#include "rapidjson/encodedstream.h"   // AutoUTFInputStream
#include <cstdio>
#include <fstream>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <iostream>

namespace report {

struct Report {
  using Events = std::vector<std::pair<std::time_t, double>>;

  Events rewardsHS;
  Events rewardsBH;
  Events positionsHS;
  Events positionsBH;

  std::vector<lib::Trade> tradesBH;
};

rapidjson::Document convertTradesToRapidJson(
    const std::vector<lib::Trade>& trades) {
  rapidjson::Document document;
  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
  document.SetArray();
  for (const auto& trade : trades) {
    rapidjson::Value value(rapidjson::kObjectType);
    value.AddMember("o", trade.openTime, allocator);
    value.AddMember("c", trade.closeTime, allocator);

    rapidjson::Value pos(rapidjson::kStringType);
    pos.SetString(trade.position.c_str(), allocator);
    value.AddMember("p", pos, allocator);

    value.AddMember("r", trade.reward, allocator);
    value.AddMember("fb", trade.fundBefore, allocator);
    value.AddMember("fa", trade.fundAfter, allocator);
    document.PushBack(value, allocator);
  }
  return document;
}

void createJsonReport(const std::vector<lib::Trade>& trades,
                      const std::string& outputMetafilePath) {
  std::ofstream ofs(outputMetafilePath);
  rapidjson::OStreamWrapper osw(ofs);

  rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
  rapidjson::Document document = convertTradesToRapidJson(trades);
  document.Accept(writer);
}

void outputEventsCSV(std::filesystem::path dir, const std::string& file,
                     const Report::Events& events) {
  auto posHS = dir / file;
  std::fstream fOut(posHS.string(), std::ios::out);
  const auto separator = ',';

  for (const auto& [time, position] : events) {
    fOut << std::noshowpos << lib::from_time_t(time) << separator;
    fOut << std::noshowpos << time << separator;
    fOut << std::setprecision(3) << std::fixed << std::showpos << position
         << '\n';
  }
}

void outputTradesCSV(std::filesystem::path dir, const std::string& file,
                     const std::vector<lib::Trade>& trades) {
  auto posHS = dir / file;
  std::fstream fOut(posHS.string(), std::ios::out);
  const auto separator = ',';

  for (const auto& trade : trades) {
    fOut << std::noshowpos << lib::from_time_t(trade.openTime) << separator;
    fOut << std::noshowpos << lib::from_time_t(trade.closeTime) << separator;

    fOut << std::noshowpos << trade.openTime << separator;
    fOut << std::noshowpos << trade.closeTime << separator;
    fOut << trade.position << separator;
    fOut << std::setprecision(3) << std::fixed << std::showpos << trade.reward
         << '\n';
  }
}

void createCSVReport(const std::string& coin,
                     const std::vector<lib::Trade>& trades,
                     const Report& report) {
  auto reportPath =
      std::filesystem::path(shared::Config::getInstance().reportDirectoryPath);

  std::string copyCoin = coin;
  for (char& to : copyCoin) {
    if (to >= 'A' && to <= 'Z') {
      to += 'a' - 'A';
    }
  }
  copyCoin += "usdt";

  reportPath /= copyCoin;
  std::filesystem::create_directories(reportPath);

  outputEventsCSV(reportPath, "positions_HS.data", report.positionsHS);
  outputEventsCSV(reportPath, "positions_BH.data", report.positionsBH);
  outputEventsCSV(reportPath, "rewards_BH.data", report.rewardsBH);
  outputEventsCSV(reportPath, "rewards_HS.data", report.rewardsHS);
  outputTradesCSV(reportPath, "trades_HS.data", trades);
  outputTradesCSV(reportPath, "trades_BH.data", report.tradesBH);
}

}  // namespace report