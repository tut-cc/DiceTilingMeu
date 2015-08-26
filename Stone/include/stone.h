#pragma once

class Stone {
  public:
    virtual ~Stone() = default;
    virtual bool at(int x, int y, int angle) const = 0;
};

