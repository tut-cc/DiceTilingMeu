#pragma once

#include "FieldEX.hpp"
#include <boost/multiprecision/cpp_int.hpp>

class BoostBitField : public FieldEX<boost::multiprecision::uint1024_t> {
private:
  boost::multiprecision::uint1024_t mat;
  boost::multiprecision::uint1024_t ok;
  boost::multiprecision::uint1024_t masks[33][2];
  bool is_first;
public:
  BoostBitField(const std::vector<std::string> &);
  BoostBitField() = delete;
  BoostBitField(const BoostBitField &) = default;
  BoostBitField(BoostBitField &&) = default;

  virtual BoostBitField& operator = (const BoostBitField &) = default;
  virtual BoostBitField& operator = (BoostBitField &&) = default;
  virtual ~BoostBitField() = default;

  virtual const boost::multiprecision::uint1024_t & raw() const noexcept override;
  virtual std::unique_ptr<FieldEX<boost::multiprecision::uint1024_t>> cloneEX() const noexcept override;
  virtual bool appliable(std::shared_ptr < StoneEX<boost::multiprecision::uint1024_t> > s, int x, int y, int reverse, int angle) const noexcept override;
  virtual void apply(std::shared_ptr < StoneEX<boost::multiprecision::uint1024_t> > s, int x, int y, int reverse, int angle) noexcept override;

  virtual bool at(int x, int y) const override;
  virtual bool appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const override;
  virtual void apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) override;
  virtual std::unique_ptr<Field> clone() const override;
};

