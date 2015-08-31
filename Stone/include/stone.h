#pragma once

class Stone {
  protected:
    int ID;
  public:
    virtual ~Stone() = default;
    virtual bool at(int x, int y, int reverse, int angle) const = 0;
    virtual int indentify() const final;
};

