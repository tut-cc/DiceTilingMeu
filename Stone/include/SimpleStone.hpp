#pragma once
#include "stone.h"

class SimpleStone : public Stone {
  private:
    int angle;
    bool mat[8][8];
    void rotate();
  public:
    SimpleStone();
    virtual ~SimpleStone() = default;
    virtual bool at(int x, int y, int angle);
};

