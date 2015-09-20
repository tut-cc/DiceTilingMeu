#pragma once
#include "field.hpp"
#include <vector>
#include <string>
#include <array>

class SimpleField : public Field {
  private:
    bool mat[32][32];
    bool ok[32][32];
    bool is_first;

	static const int tx[];
	static const int ty[];

  public:
    SimpleField(std::vector<std::string>);
    SimpleField(const bool mat[32][32], const bool ok[32][32], const decltype(history) &);
    virtual ~SimpleField() = default;
    virtual bool at(int x, int y) const override;
    virtual bool appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const override;
    virtual void apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) override;
    virtual std::unique_ptr<Field> clone() const override;

	//“K“–‚Éì‚Á‚½•]‰¿ŠÖ”
	virtual int h();
	int value = -1;
};

