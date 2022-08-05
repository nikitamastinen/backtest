#include <iomanip>
#include "datetime.hpp"
#include <ctime>

namespace lib {

time_t _mk_gmt_time(const struct tm* tm) {
  static const int month_day[12] = {0,   31,  59,  90,  120, 151,
                                    181, 212, 243, 273, 304, 334};

  int month = tm->tm_mon % 12;
  int year = tm->tm_year + tm->tm_mon / 12;
  if (month < 0) {
    month += 12;
    --year;
  }

  const int year_for_leap = (month > 1) ? year + 1 : year;

  time_t rt =
      tm->tm_sec  // Seconds
      +
      60 *
          (tm->tm_min  // Minute = 60 seconds
           +
           60 * (tm->tm_hour  // Hour = 60 minutes
                 + 24 * (month_day[month] + tm->tm_mday - 1  // Day = 24 hours
                         + 365 * (year - 70)                 // Year = 365 days
                         + (year_for_leap - 69) / 4  // Every 4 years is leap...
                         - (year_for_leap - 1) / 100  // Except centuries...
                         + (year_for_leap + 299) / 400)));  // Except 400s.
  return rt < 0 ? -1 : rt;
}

bool operator==(const DateTime& lhs, const DateTime& rhs) {
  return ((lhs.year == rhs.year) && (lhs.month == rhs.month) &&
          (lhs.day == rhs.day) && (lhs.hour == rhs.hour) &&
          (lhs.minute == rhs.minute) && (lhs.second == rhs.second));
}

bool operator!=(const DateTime& lhs, const DateTime& rhs) {
  return !(lhs == rhs);
}

bool operator<(const DateTime& lhs, const DateTime& rhs) {
  return (to_time_t(lhs) < to_time_t(rhs));
}

bool operator<=(const DateTime& lhs, const DateTime& rhs) {
  return !(lhs > rhs);
}

bool operator>(const DateTime& lhs, const DateTime& rhs) {
  return (to_time_t(lhs) > to_time_t(rhs));
}

bool operator>=(const DateTime& lhs, const DateTime& rhs) {
  return !(lhs < rhs);
}

std::ostream& operator<<(std::ostream& stream, const DateTime& date_time) {
  const auto date_separator = '/';

  stream << std::setw(4) << std::right << std::setfill('0') << std::noshowpos
         << date_time.year << date_separator << std::setw(2) << std::right
         << std::setfill('0') << std::noshowpos << date_time.month
         << date_separator << std::setw(2) << std::right << std::setfill('0')
         << std::noshowpos << date_time.day << ' ';

  const auto time_separator = ':';

  stream << std::setw(2) << std::right << std::setfill('0') << std::noshowpos
         << date_time.hour << time_separator << std::setw(2) << std::right
         << std::setfill('0') << std::noshowpos << date_time.minute
         << time_separator << std::setw(2) << std::right << std::setfill('0')
         << std::noshowpos << date_time.second;

  return stream;
}

std::time_t to_time_t(const DateTime& date_time, bool is_utc) noexcept {
  std::tm time = {static_cast<int>(date_time.second),
                  static_cast<int>(date_time.minute),
                  static_cast<int>(date_time.hour),
                  static_cast<int>(date_time.day),
                  static_cast<int>(date_time.month) - 1,
                  static_cast<int>(date_time.year) - 1900};

  if (is_utc) {
    return _mk_gmt_time(&time);
  } else {
    return std::mktime(&time);
  }
}

DateTime from_time_t(std::time_t time, bool has_milliseconds) noexcept {
  if (has_milliseconds) {
    time /= 1000;
  }

  auto tm = std::gmtime(&time);

  DateTime date_time = DateTime();

  date_time.year = static_cast<unsigned int>(tm->tm_year + 1900);
  date_time.month = static_cast<unsigned int>(tm->tm_mon + 1);
  date_time.day = static_cast<unsigned int>(tm->tm_mday);
  date_time.hour = static_cast<unsigned int>(tm->tm_hour);
  date_time.minute = static_cast<unsigned int>(tm->tm_min);
  date_time.second = static_cast<unsigned int>(tm->tm_sec);

  return date_time;
}

std::time_t duration(const DateTime& date_time_1,
                     const DateTime& date_time_2) noexcept {
  return (to_time_t(date_time_1) - to_time_t(date_time_2));
}

unsigned int day_of_week(const DateTime& date_time) noexcept {
  std::tm tm = {0,
                0,
                0,
                static_cast<int>(date_time.day),
                static_cast<int>(date_time.month) - 1,
                static_cast<int>(date_time.year) - 1900};

  std::mktime(&tm);

  return static_cast<unsigned int>(tm.tm_wday);
}

}  // namespace lib