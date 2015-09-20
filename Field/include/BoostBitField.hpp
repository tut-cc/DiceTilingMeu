#pragma once

#include "FieldEX.hpp"

template <typename T>
class BoostBitField : public FieldEX<T> {
public:
  virtual ~BoostBitField() = default;
  virtual const T & raw() const override;
};

#include "detail/BoostBitField.hpp"
