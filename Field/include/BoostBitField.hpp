#pragma once

#include "FieldEX.hpp"
#include <boost/multiprecision/cpp_int.hpp>

class BoostBitField : public FieldEX<boost::multiprecision::uint1024_t> {
private:
  boost::multiprecision::uint1024_t mat;
  boost::multiprecision::uint1024_t ok;
public:
  BoostBitField(const std::vector<std::string> &);
  BoostBitField() = delete;
  BoostBitField(const BoostBitField &) = default;
  BoostBitField(BoostBitField &&) = default;

  virtual BoostBitField& operator = (const BoostBitField &) = default;
  virtual BoostBitField& operator = (BoostBitField &&) = default;
  virtual ~BoostBitField() = default;

  virtual const boost::multiprecision::uint1024_t & raw() const override;
  virtual std::unique_ptr<FieldEX<boost::multiprecision::uint1024_t>> cloneEX() const override;

  virtual bool at(int x, int y) const override;
  virtual bool appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const override;
  virtual void apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) override;
  virtual std::unique_ptr<Field> clone() const override;
};

