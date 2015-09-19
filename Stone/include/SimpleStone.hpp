#pragma once
#include "stone.hpp"
#include <vector>
#include <string>

class SimpleStone : public Stone {
  private:
    bool mat[4][2][8][8];
    std::pair<double, double> centers[4][2];
  public:
    SimpleStone(std::vector<std::string>, int index);
    virtual ~SimpleStone() = default;
    virtual bool at(int x, int y, int reverse, int angle) const override;
    virtual std::pair<double, double> center(int reverse, int angle) const override;
};

