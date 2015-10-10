#pragma once

#include "stone.hpp"

template <typename T>
class StoneEX : public Stone {
public:
  StoneEX() = default;
  StoneEX(const StoneEX &) = default;
  virtual StoneEX& operator = (const StoneEX &) = default;
  StoneEX(StoneEX &&) = default;
  virtual StoneEX& operator = (StoneEX &&) = default;
  virtual ~StoneEX() = default;

  virtual const T & raw(int reverse, int angle) const = 0;
};
