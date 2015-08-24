#pragma once

#include "algorithm.h"
#include <iostream>

class Beamalgo : public Algorithm {
  public:
    virtual void solve() {
      std::cout << "VimBeam" << std::endl;
    }
};

