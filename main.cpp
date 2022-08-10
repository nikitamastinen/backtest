#include <iostream>

#include "backtest/backtest.hpp"
#include "backtest/controller.hpp"
#include "shared/config.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Error! Please pass the absolute path of config.json";
    return 1;
  }

  shared::registerConfig(argv[1]);

  auto backTest = backTest::BackTest();

  controller::run([&backTest](const std::string& s) {
    auto r = backTest.test(s);
    for (const auto& to : r) {
      std::cout << to.toString();
    }
  });

  return 0;
}