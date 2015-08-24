#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include "../Field/field.h"
#include "../Stone/stone.h"

template <class F, class S>
class Problem {
  public:
    virtual ~Problem() {};
    virtual std::unique_ptr<Field> get_field() = 0;
    virtual std::vector<std::weak_ptr<Stone>> get_stones() = 0;
};

