#pragma once

#include "algorithm.h"
#include "field.h"
#include "stone.h"
#include "problem.h"
#include <memory>
#include <vector>

class Ritalgo : public Algorithm {
  std::unique_ptr<Field> field;
  std::vector<std::weak_ptr<Stone>> stones;
  public:
    template <class F, class S>
    Ritalgo(std::shared_ptr<Problem<F, S>> p);
    virtual ~Ritalgo() = default;
    virtual void solve();
};

