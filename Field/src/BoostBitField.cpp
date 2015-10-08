#include "BoostBitField.hpp"

BoostBitField::BoostBitField(const std::vector<std::string>& list) : mat([&]() {
  boost::multiprecision::uint1024_t k = 1;
  boost::multiprecision::uint1024_t a = 0;
  for (auto & e : list) {
    for (auto & v : e) {
      if (v == '1') {
        a |= k;
      }
      k *= 2;
    }
  }
  return a;
}()), ok(0), is_first(true)
{
  boost::multiprecision::uint1024_t atom[2] = {};
  for (int i = 0; i < 32; ++i) {
    static const boost::multiprecision::uint1024_t ONE = 1;
    atom[0] |= ONE << i;
    atom[1] |= ONE << (i * 32);
  }
  masks[0][0] = 0;
  masks[0][1] = 0;
  for (int i = 1; i <= 32; ++i) {
    masks[i][0] = masks[i - 1][0] << 32 | atom[0];
    masks[i][1] = masks[i - 1][1] <<  1 | atom[1];
  }
}

const boost::multiprecision::uint1024_t & BoostBitField::raw() const noexcept
{
  return mat;
}

std::unique_ptr<FieldEX<boost::multiprecision::uint1024_t>> BoostBitField::cloneEX() const noexcept
{
  return std::unique_ptr<FieldEX<boost::multiprecision::uint1024_t>>(new BoostBitField(*this));
}

bool BoostBitField::appliable(std::shared_ptr < StoneEX<boost::multiprecision::uint1024_t> > s, int x, int y, int reverse, int angle) const noexcept
{
  if (std::max(std::abs(x), std::abs(y)) >= 8) {
    return false;
  }
  auto sr = s->raw(reverse, angle);
  if (x < 0) {
    if (sr & masks[std::abs(x)][0] > 0) {
      return false;
    }
    sr <<= std::abs(x);
  }
  else {
    if (sr & ~masks[8 - x][0] > 0) {
      return false;
    }
    sr >>= x;
  }
  if (y < 0) {
    if (sr & masks[std::abs(y)][1] > 0) {
      return false;
    }
    sr <<= std::abs(y) * 32;
  }
  else {
    if (sr & ~masks[8 - y][1] > 0) {
      return false;
    }
    sr >>= y * 32;
  }

  if (sr & mat > 0) {
    return false;
  }
  if (sr & ok == 0) {
    return is_first;
  }

  return true;
}

void BoostBitField::apply(std::shared_ptr<StoneEX<boost::multiprecision::uint1024_t>> s, int x, int y, int reverse, int angle) noexcept
{
  Field::apply(s, x, y, reverse, angle);
#ifdef _DEBUG
  if (!appliable(s, x, y, reverse, angle)) {
    throw std::runtime_error("cannot apply");
  }
#endif
  auto sr = s->raw(reverse, angle);
  if (x < 0) {
    sr <<= std::abs(x);
  }
  else {
    sr >>= x;
  }
  if (y < 0) {
    sr <<= std::abs(y) * 32;
  }
  else {
    sr >>= y * 32;
  }

  std::cout << "ID : " << s->identify() << " (" << x << ", " << y << ", " << reverse << ", " << angle << ")" << std::endl;
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 32; ++j) {
      std::cout << (((sr >> 32 * y) >> x) & 1);
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  mat |= sr;
  ok |= sr;

  is_first &= false;
}

bool BoostBitField::at(int x, int y) const
{
  return (((mat >> 32 * y) >> x) & 1) == 1;
}

bool BoostBitField::appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const
{
  // UNKO
  throw std::runtime_error("not implemented");
  return false;
}

void BoostBitField::apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle)
{
  // UNKO
  throw std::runtime_error("not implemented");
}

std::unique_ptr<Field> BoostBitField::clone() const
{
  return std::unique_ptr<FieldEX<boost::multiprecision::uint1024_t>>(new BoostBitField(*this));
}
