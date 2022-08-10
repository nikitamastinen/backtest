#include "config.hpp"

namespace shared {

Config* Config::instance = nullptr;

void registerConfig(const std::string& configPath) {
  shared::Config::getInstance(configPath);
}

} //namespace shared