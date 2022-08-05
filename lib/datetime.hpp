#pragma once

#include <ctime>
#include <ostream>
#include <cstdio>
#include <ctime>

namespace lib {
  struct DateTime {
    unsigned int year;
    unsigned int month;
    unsigned int day;

    unsigned int hour;
    unsigned int minute;
    unsigned int second;
  };

  bool operator==(const DateTime &lhs, const DateTime &rhs);

  bool operator!=(const DateTime &lhs, const DateTime &rhs);

  bool operator<(const DateTime &lhs, const DateTime &rhs);

  bool operator<=(const DateTime &lhs, const DateTime &rhs);

  bool operator>(const DateTime &lhs, const DateTime &rhs);

  bool operator>=(const DateTime &lhs, const DateTime &rhs);

  std::ostream &operator<<(std::ostream &stream, const DateTime &date_time);

  std::time_t to_time_t(const DateTime &date_time, bool is_utc = true) noexcept;

  DateTime from_time_t(std::time_t time, bool has_milliseconds = false) noexcept;

  std::time_t duration(const DateTime &date_time_1, const DateTime &date_time_2) noexcept;

  unsigned int day_of_week(const DateTime &date_time) noexcept;
}