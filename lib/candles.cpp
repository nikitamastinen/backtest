#include "candles.hpp"
#include <iomanip>

#include "../shared/config.hpp"

namespace lib {

struct Config {
  const std::string system = "system";

  const std::string need_launch = "need_launch";
  const std::string launch = "launch";
  const std::string hh = "hh";
  const std::string mm = "mm";
  const std::string interval = "interval";
  const std::string strategy = "strategy";
  const std::string trailing = "trailing";

  const std::string source = "source";

  const std::string asset = "BTCUSDT";
  const std::string timeframe = "timeframe";
  //   const std::string strategy          = "strategy";
  //   const std::string trailing          = "trailing";
  //
  //   const std::string strategy          = "strategy";
  const std::string indicator = "indicator";
  const std::string bbw = "bbw";
  const std::string timesteps = "timesteps";
  const std::string view = "view";
  const std::string threshold = "threshold";
  const std::string ema = "ema";
  const std::string is_dema = "is_dema";
  //   const std::string timesteps         = "timesteps";
  const std::string bias = "bias";
  //   const std::string view              = "view";
  const std::string need_optimization = "need_optimization";

  const std::string backtest = "backtest";
  const std::string last_year = "last_year";
  const std::string last_month = "last_month";
  const std::string has_reinvestment = "has_reinvestment";
  const std::string investment = "investment";
  const std::string commission = "commission";
  const std::string need_report = "need_report";
};

Candle parseCandles(const std::string& line) {
  try {
    CandleParser<std::string::const_iterator> parser;

    auto first = std::begin(line);
    auto last = std::end(line);

    Candle kline{};

    auto result = boost::spirit::qi::phrase_parse(
        first, last, parser, boost::spirit::qi::blank, kline);

    if (result) {
      return kline;
    } else {
      throw std::runtime_error("lib::parseCandles: can't parse candles");
    }
  } catch (const std::exception& exception) {
    std::cerr << "parse_candles\n";
    throw std::runtime_error("lib::parseCandles: can't parse candles");
  }
}

std::string make_klines_file_name(const std::string& coin, std::time_t year,
                                  std::time_t month,
                                  const std::string& timeframe) {
  try {
    const auto separator = '-';

    std::stringstream sout;
    Config m_config;
    sout << coin << separator << timeframe << separator << year << separator
         << std::setw(2) << std::setfill('0') << std::right << month << ".csv";
    return sout.str();
  } catch (const std::exception& exception) {
    throw std::runtime_error("cannot make candles filename");
  }
}

std::vector<lib::Candle> loadCandles(const std::string& coin,
                                     const std::string& timeframe) {
  const auto epsilon = std::numeric_limits<double>::epsilon();

  std::vector<Candle> klines;

  std::time_t first_year = 2022;
  std::time_t first_month = 1;

  Config m_config;
  std::time_t last_year = 2022;
  std::time_t last_month = 5;

  for (std::time_t year = 2015; year <= 2022; ++year) {
    for (std::time_t month = 1; month <= 12; ++month) {
      if (year < first_year || year == first_year && month < first_month ||
          year > last_year || year == last_year && month > last_month) {
        continue;
      }

      std::filesystem::path path{R"(D:\business\backtester\data)"};
      path /= make_klines_file_name(coin, year, month, timeframe);

      if (!std::filesystem::exists(path)) {
        throw std::runtime_error(
            "loadCandles: file with candles doesn't exists");
      }
      std::fstream fin(path.string(), std::ios::in);

      if (!fin) {
        throw std::runtime_error("loadCandles: can't open file");
      }

      std::string line;

      while (std::getline(fin, line)) {
        auto kline = parseCandles(line);

        if (kline.price_open < epsilon) {
          throw std::domain_error("required: (price_open > 0.0)");
        }

        if (kline.price_high < epsilon) {
          throw std::domain_error("required: (price_high > 0.0)");
        }

        if (kline.price_low < epsilon) {
          throw std::domain_error("required: (price_low > 0.0)");
        }

        if (kline.price_close < epsilon) {
          throw std::domain_error("required: (price_close > 0.0)");
        }

        klines.push_back(kline);
      }
    }
  }

  std::sort(std::begin(klines), std::end(klines),
            [](const auto& lhs, const auto& rhs) {
              return (lhs.time_open < rhs.time_open);
            });

  return klines;
}

std::map<std::string, std::vector<lib::Candle>> readData() {
  std::ifstream input(shared::Config::getInstance().getPairsFilePath());

  std::map<std::string, std::vector<lib::Candle>> result;

  for (std::string line; getline(input, line);) {
    auto current = loadCandles(line, "5m");
    for (auto& to : current) {
      to.time_close /= 1000;
      to.time_open /= 1000;
    }
    result[line.substr(0, line.size() - 4)] = std::move(current);
    std::cout << line << std::endl;
  }
  return result;
}

}  // namespace lib
