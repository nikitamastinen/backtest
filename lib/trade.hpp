#pragma once

#include <ctime>
#include <string>

namespace lib {

class Trade {
 public:
  std::time_t openTime;
  std::time_t closeTime;
  std::string position;

  double fundBefore;
  double fundAfter;
  double reward;

 public:
  std::string toString() const {
    std::string result = "open: " + std::to_string(openTime);
    result += "\nclose: " + std::to_string(closeTime);
    result += "\nposition: " + position;
    result += "\nfundBefore: " + std::to_string(fundBefore);
    result += "\nfundAfter: " + std::to_string(fundAfter);
    result += "\nreward: " + std::to_string(reward);
  }

  void print() const {
    std::cout << toString() << std::endl << std::endl;
  }
};

}  // namespace lib