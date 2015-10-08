#pragma once

#include "field.hpp"
#include "StoneEX.hpp"

template <typename T>
class FieldEX : public Field {
public:
  FieldEX() = default;
  FieldEX(const FieldEX &) = default;
  virtual FieldEX& operator = (const FieldEX &) = default;
  FieldEX(FieldEX &&) = default;
  virtual FieldEX& operator = (FieldEX &&) = default;
  virtual ~FieldEX() = default;

  virtual const T & raw() const noexcept = 0;
  virtual bool appliable(std::shared_ptr < StoneEX<T> > s, int x, int y, int reverse, int angle) const noexcept = 0;
  virtual void apply(std::shared_ptr < StoneEX<T> > s, int x, int y, int reverse, int angle) noexcept = 0;
  virtual std::unique_ptr<FieldEX> cloneEX() const noexcept = 0;
};
