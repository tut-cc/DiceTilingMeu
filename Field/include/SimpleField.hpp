#pragma once
#include "field.h"
#include <vector>
#include <string>

class SimpleField : public Field {
  private:
    bool mat[32][32];
    bool ok[32][32];
  public:
    SimpleField(std::vector<std::string>);
    SimpleField(const bool mat[32][32]);
    virtual ~SimpleField() = default;
    virtual bool at(int x, int y) const override;
    virtual bool appliable(std::weak_ptr<Stone> s, int x, int y, int reverse, int angle) const override;
    virtual void apply(std::weak_ptr<Stone> s, int x, int y, int reverse, int angle) override;
    virtual std::unique_ptr<Field> clone() const override;
};

