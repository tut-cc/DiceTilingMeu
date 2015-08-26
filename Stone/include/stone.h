#pragma once

class Stone {
  public:
    virtual ~Stone();
    virtual bool at(int x, int y, int angle) = 0;
};

