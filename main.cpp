#include <iostream>

#include "backtest/backtest.hpp"
#include "backtest/controller.hpp"
#include "shared/config.hpp"

int main() {
  shared::registerConfig(R"(D:\business\backtester\config.json)");

  auto backTest = backTest::BackTest();

  controller::run([&backTest](const std::string& s) {
    auto r = backTest.test(s);
    for (const auto& to : r) {
      std::cout << to.toString();
    }
  });
  return 0;
}