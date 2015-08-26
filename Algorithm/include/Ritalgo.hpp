#pragma once

#include "algorithm.hpp"
#include "field.hpp"
#include "stone.hpp"
#include "problem.hpp"
#include <memory>
#include <vector>

template <class F, class S>
class Ritalgo : public Algorithm {
  std::unique_ptr<Field> field;
  std::vector<std::shared_ptr<Stone>> stones;
  public:
    Ritalgo(std::shared_ptr<Problem> p);
    virtual ~Ritalgo() = default;
    virtual void solve();
};

#include "detail/Ritalgo.hpp"

