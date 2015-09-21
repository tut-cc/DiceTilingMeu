#pragma once

#include "field.hpp"

template <typename T>
class FieldEX : public Field {
public:
  FieldEX() = default;
  FieldEX(const FieldEX &) = default;
  virtual FieldEX& operator = (const FieldEX &) = default;
  FieldEX(FieldEX &&) = default;
  virtual FieldEX& operator = (FieldEX &&) = default;
  virtual ~FieldEX() = default;

  virtual const T & raw() const = 0;
  virtual std::unique_ptr<FieldEX> cloneEX() const = 0;
};
