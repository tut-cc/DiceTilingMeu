#pragma once

#include "StoneEX.hpp"
#include <boost/multiprecision/cpp_int.hpp>

class BoostBitStone : public StoneEX<boost::multiprecision::uint1024_t> {
private:
  boost::multiprecision::uint1024_t mat[4][2];
  std::pair<double, double> centers[4][2];

public:
  BoostBitStone(const std::vector<std::string> &, int);
  BoostBitStone() = delete;
  BoostBitStone(const BoostBitStone &) = default;
  BoostBitStone(BoostBitStone &&) = default;

  virtual BoostBitStone& operator = (const BoostBitStone &) = default;
  virtual BoostBitStone& operator = (BoostBitStone &&) = default;
  virtual ~BoostBitStone() = default;

  virtual const boost::multiprecision::uint1024_t & raw(int reverse, int angle) const override;

  virtual bool at(int x, int y, int reverse, int angle) const override;
  virtual std::pair<double, double> center(int reverse, int angle) const override;
};
