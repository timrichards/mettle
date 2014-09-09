#include "filters.hpp"

namespace mettle {
  filter_result attr_filter::operator ()(const attributes &attrs) const {
    using detail::join;

    std::set<const attr_instance*> explicitly_shown;
    for(const auto &f : filters_) {
      auto i = attrs.find(f.attribute);
      const attr_instance *attr = i == attrs.end() ? nullptr: &*i;

      if(!f.func(attr))
        return {test_action::hide, attr ? join(attr->value, ", ") : ""};
      else if(attr)
        explicitly_shown.insert(attr);
    }
    for(const auto &attr : attrs) {
      if(attr.attribute.action() == test_action::skip &&
         !explicitly_shown.count(&attr))
        return {test_action::skip, join(attr.value, ", ")};
    }
    return {test_action::run, ""};
  }

  filter_result attr_filter_set::operator ()(const attributes &attrs) const {
    // Pretend we always have a default filter, if nothing else.
    if(filters_.empty())
      return default_attr_filter{}(attrs);

    bool set = false;
    filter_result result;
    for(const auto &f : filters_) {
      auto curr = f(attrs);
      switch(curr.first) {
      case test_action::run:
        return curr;
      case test_action::skip:
        if(!set || result.first == test_action::hide) {
          result = curr;
          set = true;
        }
        break;
      case test_action::hide:
        if(!set) {
          result = curr;
          set = true;
        }
        break;
      }
    }
    return result;
  }
}