#include <iostream>
#include "backtest/backtest.hpp"
#include "backtest/controller.hpp"


void callBackTest(const std::string&) {
  std::cout << "kek" << std::endl;
}

int main() {
  auto backTest = backTest::BackTest();
  controller::run([&backTest](const std::string& s) {
    auto r = backTest.test(s);
    for (const auto& to : r) {
      std::cout << to.toString();
    }
  });
  return 0;
}