#include <vector>
#include <map>
#include "datetime.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/config.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <filesystem>

#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif  // #ifdef BOOST_HAS_PRAGMA_ONCE

namespace lib {
struct Input {
  using inputs_container_t = std::vector<Input>;
  using indicators_container_t =
      std::map<std::string, std::map<std::string, double>>;

  indicators_container_t indicators;

  DateTime date_time_open;
  DateTime date_time_close;

  unsigned int day;
  unsigned int hour;

  double price_open;
  double price_high;
  double price_low;
  double price_close;

  double volume_base;
};

struct Candle {
  std::time_t time_open;

  double price_open;
  double price_high;
  double price_low;
  double price_close;

  double volume_base;

  std::time_t time_close;

  double volume_quote;

  std::size_t n_trades;

  double volume_buy_base;
  double volume_buy_quote;

  double ignore;

  [[nodiscard]] std::string toString() const {
    std::string result = "priceOpen: " + std::to_string(price_open);
    result += "\npriceHigh: " + std::to_string(price_high);
    result += "\npriceLow: " + std::to_string(price_low);
    result += "\npriceClose: " + std::to_string(price_close);

    result += "\ntimeOpen: " + std::to_string(time_open);
    result += "\ntimeClose: " + std::to_string(time_close);

    return result;
  }

  [[maybe_unused]] void print() const {
    std::cout << toString() << std::endl;
  }
};

template < typename Iterator >
class CandleParser
    : public boost::spirit::qi::grammar <
                         Iterator, Candle(), boost::spirit::qi::blank_type >
{
 private:

  using rule_t = boost::spirit::qi::rule <
      Iterator, Candle(), boost::spirit::qi::blank_type > ;

 public:
  CandleParser() : CandleParser::base_type(start)
  {
    start %=
        boost::spirit::qi::long_long  >> separator >> // time_open
        boost::spirit::qi::double_    >> separator >> // price_open
        boost::spirit::qi::double_    >> separator >> // price_high
        boost::spirit::qi::double_    >> separator >> // price_low
        boost::spirit::qi::double_    >> separator >> // price_close
        boost::spirit::qi::double_    >> separator >> // volume_base
        boost::spirit::qi::long_long  >> separator >> // time_close
        boost::spirit::qi::double_    >> separator >> // volume_quote
        boost::spirit::qi::ulong_long >> separator >> // n_trades
        boost::spirit::qi::double_    >> separator >> // volume_buy_base
        boost::spirit::qi::double_    >> separator >> // volume_buy_quote
        boost::spirit::qi::double_;                   // ignore

  }

  ~CandleParser() noexcept = default;

 private:

  const char separator = ',';

 private:

  rule_t start;
};
}  // namespace lib

BOOST_FUSION_ADAPT_STRUCT(
    lib::Candle,
    (std::time_t, time_open)(double, price_open)(double, price_high)(
        double, price_low)(double, price_close)(double, volume_base)(
        std::time_t, time_close)(double, volume_quote)(std::size_t, n_trades)(
        double, volume_buy_base)(double, volume_buy_quote)(double, ignore))