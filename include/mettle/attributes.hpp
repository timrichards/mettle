#ifndef INC_METTLE_ATTRIBUTES_HPP
#define INC_METTLE_ATTRIBUTES_HPP

#include <cassert>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

#include "test_info.hpp"

namespace mettle {

class attr_base;

struct attr_instance {
  using value_type = std::set<std::string>;

  const attr_base &attribute;
  const value_type value;
};

class attr_base {
protected:
  constexpr attr_base(const char *name, test_action action = test_action::run)
    : name_(name), action_(action) {
    if(action != test_action::run && action != test_action::skip) {
      throw std::invalid_argument(
        "attribute's action must be \"run\" or \"skip\""
      );
    }
  }
  ~attr_base() = default;
public:
  std::string name() const {
    return name_;
  }

  test_action action() const {
    return action_;
  }

  virtual const attr_instance
  compose(const attr_instance &lhs, const attr_instance &rhs) const {
    assert(&lhs.attribute == this && &rhs.attribute == this);
    (void)rhs;
    return lhs;
  }
private:
  const char *name_;
  test_action action_;
};

namespace detail {
  struct attr_less {
    using is_transparent = void;

    bool operator ()(const attr_instance &lhs, const attr_instance &rhs) const {
      return lhs.attribute.name() < rhs.attribute.name();
    }

    bool operator ()(const attr_instance &lhs, const std::string &rhs) const {
      return lhs.attribute.name() < rhs;
    }

    bool operator ()(const std::string &lhs, const attr_instance &rhs) const {
      return lhs < rhs.attribute.name();
    }
  };
}

class bool_attr : public attr_base {
public:
  constexpr bool_attr(const char *name, test_action action = test_action::run)
    : attr_base(name, action) {}

  operator const attr_instance() const {
    return attr_instance{*this, {}};
  }

  template<typename T>
  const attr_instance operator ()(T &&comment) const {
    return attr_instance{*this, {std::forward<T>(comment)}};
  }
};

class string_attr : public attr_base {
public:
  constexpr string_attr(const char *name)
    : attr_base(name) {}

  template<typename T>
  const attr_instance operator ()(T &&value) const {
    return attr_instance{*this, {std::forward<T>(value)}};
  }
};

class list_attr : public attr_base {
public:
  constexpr list_attr(const char *name)
    : attr_base(name) {}

  template<typename ...T>
  const attr_instance operator ()(T &&...args) const {
    return attr_instance{*this, {std::forward<T>(args)...}};
  }

  const attr_instance
  compose(const attr_instance &lhs, const attr_instance &rhs) const {
    assert(&lhs.attribute == this && &rhs.attribute == this);
    attr_instance::value_type merged;
    std::set_union(
      lhs.value.begin(), lhs.value.end(),
      rhs.value.begin(), rhs.value.end(),
      std::inserter(merged, merged.begin())
    );
    return attr_instance{*this, std::move(merged)};
  }
};

using attributes = std::set<const attr_instance, detail::attr_less>;

namespace detail {
  template<typename Input1, typename Input2, typename Output, typename Compare,
           typename Merge>
  Output merge_union(Input1 first1, Input1 last1, Input2 first2, Input2 last2,
                     Output result, Compare comp, Merge merge) {
    for(; first1 != last1; ++result) {
      if(first2 == last2)
        return std::copy(first1, last1, result);

      if(comp(*first1, *first2))
        *result = *first1++;
      else if(comp(*first2, *first1))
        *result = *first2++;
      else
        *result = merge(*first1++, *first2++);
    }
    return std::copy(first2, last2, result);
  }
}

inline attr_instance unite(const attr_instance &lhs, const attr_instance &rhs) {
  if(&lhs.attribute != &rhs.attribute)
    throw std::invalid_argument("mismatched attributes");
  return lhs.attribute.compose(lhs, rhs);
}

inline attributes unite(const attributes &lhs, const attributes &rhs) {
  attributes all_attrs;
  detail::merge_union(
    lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
    std::inserter(all_attrs, all_attrs.begin()), detail::attr_less(),
    [](const attr_instance &lhs, const attr_instance &rhs) {
      return unite(lhs, rhs);
    }
  );
  return all_attrs;
}

constexpr bool_attr skip("skip", test_action::skip);

} // namespace mettle

#endif
