#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include "../../Field/include/field.h"
#include "../../Stone/include/stone.h"

template <class F, class S>
class Problem {
  static_assert(std::is_base_of<Field, F>::value, "class F maybe not base of Field");
  static_assert(std::is_base_of<Stone, S>::value, "class S maybe not base of Stone");
  public:
    virtual ~Problem() {};
    virtual std::unique_ptr<Field> get_field() = 0;
    virtual std::vector<std::weak_ptr<Stone>> get_stones() = 0;
};

