#ifndef INC_METTLE_DRIVER_LOG_TERM_HPP
#define INC_METTLE_DRIVER_LOG_TERM_HPP

#include <cstdint>
#include <sstream>
#include <string>
#include <type_traits>

namespace term {

namespace detail {
  inline int stream_flag() {
    static int flag = std::ios_base::xalloc();
    return flag;
  }

  template<typename T, typename ...>
  struct are_same : std::true_type {};

  template<typename T, typename First, typename ...Rest>
  struct are_same<T, First, Rest...> : std::integral_constant<
    bool, std::is_same<T, First>::value && are_same<T, Rest...>::value
  > {};
}

inline void enable(std::ios_base &ios, bool enabled) {
  ios.iword(detail::stream_flag()) = enabled;
}

enum class sgr {
  reset       = 0,
  bold        = 1,
  faint       = 2,
  italic      = 3,
  underline   = 4,
  blink       = 5,
  blink_fast  = 6,
  inverse     = 7,
  conceal     = 8,
  crossed_out = 9
};

enum class color {
  black   = 0,
  red     = 1,
  green   = 2,
  yellow  = 3,
  blue    = 4,
  magenta = 5,
  cyan    = 6,
  white   = 7,
  normal  = 9
};

inline sgr fg(color c) {
  return static_cast<sgr>(30 + static_cast<std::size_t>(c));
}

inline sgr bg(color c) {
  return static_cast<sgr>(40 + static_cast<std::size_t>(c));
}

class format {
  friend std::ostream & operator <<(std::ostream &, const format &);
public:
  template<typename ...Args>
  explicit format(Args &&...args) {
    static_assert(sizeof...(Args) > 0,
                  "term::format must have at least one argument");
    static_assert(detail::are_same<sgr, Args...>::value,
                  "term::format's arguments must be of type term::sgr");
    int values[] = {static_cast<int>(std::forward<Args>(args))...};

    std::ostringstream ss;
    ss << "\033[" << values[0];
    for(std::size_t i = 1; i != sizeof...(Args); i++)
      ss << ";" << values[i];
    ss << "m";
    string_ = ss.str();
  }
private:
  std::string string_;
};

inline format reset() {
  return format(sgr::reset);
}

inline std::ostream & operator <<(std::ostream &o, const format &fmt) {
  if(o.iword(detail::stream_flag()))
    return o << fmt.string_;
  return o;
}

} // namespace term

#endif