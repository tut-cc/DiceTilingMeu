#pragma once
#include "field.h"

class SimpleField : public Field {
  private:
    bool mat[32][32];
  public:
    SimpleField();
    SimpleField(const bool mat[32][32]);
    virtual bool at(int x, int y) const;
    virtual bool appliable(std::weak_ptr<Stone> s, int x, int y, int rotate) const;
    virtual void apply(std::weak_ptr<Stone> s, int x, int y, int rotate);
    virtual std::unique_ptr<Field> clone() const;
};

