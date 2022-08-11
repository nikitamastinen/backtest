#pragma once

#include <vector>
#include "../lib/trade.hpp"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"  // FileReadStream
#include "rapidjson/encodedstream.h"   // AutoUTFInputStream
#include <cstdio>
#include <fstream>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <iostream>

namespace report {

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
    std::cout << document.Size() << std::endl;
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

void createCSVReport() {

}

}  // namespace report