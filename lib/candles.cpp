#include "candles.hpp"

namespace lib {

Kline parse_kline(const std::string& line) {
  try {
    Kline_Parser<std::string::const_iterator> parser;

    auto first = std::begin(line);
    auto last = std::end(line);

    //    Kline kline;
    //
    //    auto result = boost::spirit::qi::phrase_parse(
    //        first, last, parser, boost::spirit::qi::blank, kline);
    //
    //    if (result) {
    //      return kline;
    //    } else {
    //      throw source_exception("cannot parse line " + line);
    //    }
  } catch (const std::exception& exception) {
    // shared::catch_handler<source_exception>(logger, exception);
    std::cerr << "parse_candles\n";
  }
}

void load_klines(const std::string& timeframe) {
  //  try {
  //    const auto epsilon = std::numeric_limits<double>::epsilon();
  //
  //    klines_container_t klines;
  //
  //    std::time_t first_year = 2021;
  //    std::time_t first_month = 1;
  //
  //    std::time_t last_year = m_config.source.backtest.last_year;
  //    std::time_t last_month = m_config.source.backtest.last_month;
  //
  //    for (std::time_t year = 2015; year <= 2022; ++year) {
  //      for (std::time_t month = 1; month <= 12; ++month) {
  //        if (year < first_year || year == first_year && month < first_month
  //        ||
  //            year > last_year || year == last_year && month > last_month) {
  //          continue;
  //        }
  //
  //        auto path = Data::Directory::klines;
  //
  //        ((path /= m_asset) /= timeframe) /=
  //            make_klines_file_name(year, month, timeframe);
  //
  //        if (!std::filesystem::exists(path)) {
  //          LOGGER_WRITE_DEBUG(logger,
  //                             "file " + path.string() + " doesn't exist");
  //
  //          continue;
  //        }
  //
  //        std::fstream fin(path.string(), std::ios::in);
  //
  //        if (!fin) {
  //          throw source_exception("cannot open file " + path.string());
  //        }
  //
  //        std::string line;
  //
  //        while (std::getline(fin, line)) {
  //          auto kline = parse_kline(line);
  //
  //          if (kline.price_open < epsilon) {
  //            throw std::domain_error("required: (price_open > 0.0)");
  //          }
  //
  //          if (kline.price_high < epsilon) {
  //            throw std::domain_error("required: (price_high > 0.0)");
  //          }
  //
  //          if (kline.price_low < epsilon) {
  //            throw std::domain_error("required: (price_low > 0.0)");
  //          }
  //
  //          if (kline.price_close < epsilon) {
  //            throw std::domain_error("required: (price_close > 0.0)");
  //          }
  //
  //          klines.push_back(std::move(kline));
  //        }
  //      }
  //    }
  //
  //    std::sort(std::begin(klines), std::end(klines),
  //              [](const auto& lhs, const auto& rhs) {
  //                return (lhs.time_open < rhs.time_open);
  //              });
  //
  //    return klines;
  //  } catch (const std::exception& exception) {
  //    //shared::catch_handler<source_exception>(logger, exception);
  //    std::cerr << "load_candles\n";
  //}
}

}  // namespace lib
