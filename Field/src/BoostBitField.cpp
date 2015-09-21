
#include "BoostBitField.hpp"



BoostBitField::BoostBitField(const std::vector<std::string>&)
{
}

const boost::multiprecision::uint1024_t & BoostBitField::raw() const
{
  boost::multiprecision::uint1024_t v;
  return v;
}

std::unique_ptr<FieldEX<boost::multiprecision::uint1024_t>> BoostBitField::cloneEX() const
{
  return std::unique_ptr<FieldEX<boost::multiprecision::uint1024_t>>(new BoostBitField(*this));
}


bool BoostBitField::at(int x, int y) const
{
  return false;
}

bool BoostBitField::appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const
{
  return false;
}

void BoostBitField::apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle)
{
}

std::unique_ptr<Field> BoostBitField::clone() const
{
  return std::unique_ptr<FieldEX<boost::multiprecision::uint1024_t>>(new BoostBitField(*this));
}
