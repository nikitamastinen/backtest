#include <iostream>

#include "backtest/backtest.hpp"
#include "backtest/controller.hpp"
#include "shared/config.hpp"
#include "report/report.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Error! Please pass the absolute path of config.json";
    return 1;
  }

  shared::registerConfig(argv[1]);

  auto backTest = backTest::BackTest();

  controller::run([&backTest](const std::string& swapFilePath) {
    auto r = backTest.test(swapFilePath);

    report::createJsonReport(
        r, swapFilePath.substr(0, swapFilePath.size() - 5) + "_metafile.json");

    if (shared::Config::getInstance().isLoadReport) {
      report::createCSVReport(backTest.coin, r, backTest.report);
    }

    for (const auto& to : r) {
      std::cout << to.toString() << std::endl << std::endl;
    }

    std::cout << std::endl << std::endl;
  });

  return 0;
}