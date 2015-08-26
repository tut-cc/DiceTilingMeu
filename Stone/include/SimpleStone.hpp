#pragma once
#include "stone.h"
#include <vector>
#include <string>

class SimpleStone : public Stone {
  private:
    bool mat[4][8][8];
  public:
    SimpleStone(std::vector<std::string>);
    virtual ~SimpleStone() = default;
    virtual bool at(int x, int y, int angle) const;
};

